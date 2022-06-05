#ifndef UART_INTF_H
#define UART_INTF_H

#include "systemc.h"
#include <assert.h>
#include "globals.hpp"

SC_MODULE(uart_intf) {
  //dut
  sc_in<bool> clk_i; // SC_THREAD does not trigger if not declared as sc_in 
  sc_signal<bool> rst_i;
  sc_signal<uint32_t> bauds_lim_i;
  sc_signal<bool> rx_i;
  sc_signal<uint32_t> tx_data_i;
  sc_signal<bool> rx_en_i;
  sc_signal<bool> tx_en_i;
  sc_signal<bool> parity_en_i;
  sc_signal<bool> parity_odd_i;
  //outs
  sc_signal<bool> tx_done_o;
  sc_signal<bool> tx_o;
  sc_signal<uint32_t> rx_data_o;
  sc_signal<bool> rx_valid_o;

  //tb
  sc_signal<bool> baud_tick, baud_tick_x16; //dut internal signal
  sc_time baud_tick_ts, baud_tick_p;
  uint32_t baud_tick_rate, target_baudrate, actual_baudrate;
  sc_time target_baud_period;
  uint8_t tx_data_expected, tx_data_measured;

  // other
  sc_time clk_time;
  

  SC_HAS_PROCESS(uart_intf);
  uart_intf(sc_module_name n, sc_time clk_time) : sc_module(n)
  {
    /* DUT signals */
    rst_i = 0;
    bauds_lim_i = 0;
    rx_i = 0;
    tx_data_i = 0;
    rx_en_i = 0;
    tx_en_i = 0;
    parity_en_i = 0;
    parity_odd_i = 0;

    /* Other */
    this->clk_time = clk_time;
    
    // baud tick monitor
    baud_tick = 0;
    baud_tick_x16 = 0;
    baud_tick_ts = SC_ZERO_TIME;
    baud_tick_p = SC_ZERO_TIME;
    baud_tick_rate = 0;

    // tx data monitor
    tx_data_measured = 0;

    // processes
    SC_THREAD(baud_tick_monitor_thread);
    sensitive << baud_tick_x16 << baud_tick; // static sensitivity

    SC_THREAD(tx_data_monitor_thread);
    sensitive << clk_i.pos(); // static sensitivity

  }

  void reset(void) {
    sc_core::sc_time T{100, SC_NS};
    rst_i = 1;
    wait(clk_time*20);
    rst_i = 0;
  }

  void wait_periods(int c) {
    wait(c * clk_time);
  }

  void wait_cycles(int c) {
    for (size_t i = 0; i < c; i++)
    {
      wait(clk_i.posedge_event());  
    }
  }

  void load_baudrate(uint32_t baudrate){
    double bauds_lim_temp;
    target_baudrate = baudrate;
    LOG("Target baudrate : " + std::to_string(baudrate));
    /* baudrate calculation */
    target_baud_period = sc_time(1, SC_SEC) / baudrate;
    LOG("target_baud_period: " + target_baud_period.to_string());

    bauds_lim_temp = (1/clk_time.to_seconds()) / (baudrate*16);
    bauds_lim_i = bauds_lim_temp; // write dut port
    LOG("bauds_lim_i is set to" + std::to_string(bauds_lim_temp));
  }

  uint8_t reverse(uint8_t b) {
   b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
   b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
   b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
   return b;
  }

  void check_parity_bit(bool mesaured, bool expected, bool parity_odd)
  {
    bool result = 0;
    bool cmp = parity_odd ? (expected ^ parity_odd) : expected;

    if (mesaured == cmp) result = 1;
    
    assert(mesaured == cmp);

    if (result) LOG("Parity does match!!!");
    else LOG("Parity does not match!!!");
  }

  void tx_data_monitor_thread() {
    unsigned int bit_cnt;
    bool parity_en, parity_odd;
    bool parity_expected;
    while(true) {

        while(!tx_en_i) wait(); // wait until tx_en is observed at sampling edge
        tx_data_expected = tx_data_i; // register tx_data
        LOG("Start xfer!!!");
        LOG("tx_data_expected : " + std::to_string(tx_data_expected));
        tx_data_measured = 0;

        // register xfer config
        bit_cnt = parity_en_i ? 9 : 8;
        parity_en = parity_en_i; 
        parity_odd = parity_odd_i;
        parity_expected = 0;

        wait(target_baud_period / 2); // get to the middle of start bit
        if(tx_o) LOG("Error !!! this has to be start bit");

        for (uint i = 1; i <= bit_cnt; i++)
        {
          wait(target_baud_period);
          if (i == 9) // 9 is only reachable when parity is enabled 
          { 
            check_parity_bit(tx_o, parity_expected, parity_odd);
          }
          else
          {
            // shift new data from MSB
            tx_data_measured = tx_data_measured >> 1;
            tx_data_measured |= tx_o << 7; // bitwise or to set the 8th bit
            if(tx_o) parity_expected ^= 1; // count number of bits with a value of 1
          }
        }
        wait(target_baud_period); // move to stop bit
        if(!tx_o) LOG("Error !!! this has to be stop bit at");

        if (tx_data_expected != tx_data_measured)
        {
          LOG("Test failed");
          LOG(std::string("tx_data_expected:") + std::to_string(tx_data_expected) + " vs " + 
                "tx_data_measured:" + std::to_string(tx_data_measured));
          LOG("parity_en:" +  std::to_string(parity_en) + " parity_odd:" + std::to_string(parity_odd));
          LOG("actual baud period:" + (baud_tick_p*16).to_string());
          LOG("expected baud period : " + std::to_string(1e12 / target_baudrate));
          LOG("actual baudrate : " + std::to_string(baud_tick_rate/16));
          LOG("baudrate error percent : " + std::to_string(100.0 * (target_baudrate - actual_baudrate) / (target_baudrate)));
        }
        assert(tx_data_expected == tx_data_measured);
        LOG("Transaction succeeded... End of XFER");
        wait(tx_done_o.posedge_event());
        wait(); // wait 1 cycle
    }
  }

  void baud_tick_monitor_thread(){

    while(true)
    {
      if (baud_tick_x16)
      {
        /* Calculate, dt and tick rate */
        if(baud_tick_ts != SC_ZERO_TIME){ // if we have already captured the first pulse
          baud_tick_p = sc_time_stamp() - baud_tick_ts;
          baud_tick_rate = 1 / baud_tick_p.to_seconds();
        }
        baud_tick_ts = sc_time_stamp(); // register current pulse timestamp
        actual_baudrate = baud_tick_rate / 16;

        /* Check for expected tick rate*/
        // std::cout << "bauds_lim_i : " << bauds_lim_i << endl;
        // std::cout << "baud_tick is triggered at " << sc_time_stamp() << endl;
        // std::cout << "baud_tick_p : " << baud_tick_p << endl;
        // std::cout << "baud_tick_rate : " << (baud_tick_rate/1e6) << "Mhz" << endl;
      }
      
      if(baud_tick && target_baudrate != 0) 
      {
        // std::cout << "actual baud period: " << baud_tick_p*16 << endl;
        // std::cout << "expected baud period : " << (1e12 / target_baudrate) << endl;
        // std::cout << "actual baudrate : " << baud_tick_rate/16 << endl;
        // std::cout << "baudrate error percent : " << 100.0 * (target_baudrate - actual_baudrate) / (target_baudrate) << endl;
      }
      wait();
    }
  }
};

#endif // UART_INTF_H