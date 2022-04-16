#ifndef UART_INTF_H
#define UART_INTF_H

#include "systemc.h"

SC_MODULE(uart_intf) {

  sc_signal<bool> clk_i;
  sc_signal<bool> rst_i;
  sc_signal<bool> rx_i;
  sc_signal<bool> wr_data_i;
  sc_signal<bool> rx_en;
  sc_signal<bool> tx_en;
  sc_signal<bool> parity_en;
  sc_signal<bool> parity_odd;
  sc_signal<bool> data_order;
  sc_signal<bool> polarity;
  sc_signal<bool> rd_data_o;
  sc_signal<bool> tx_o;
  sc_signal<bool> rx_valid;
  sc_signal<uint32_t> bauds_lim;

  sc_signal<bool> baud_tick_int; //dut internal signal
  sc_time baud_tick_ts, baud_tick_p;
  uint baud_tick_rate;
  

  SC_CTOR(uart_intf) {
    clk_i = 0;
    rst_i = 0;
    rx_i = 0;
    wr_data_i = 0;
    rx_en = 0;
    tx_en = 0;
    parity_en = 0;
    parity_odd = 0;
    data_order = 0;
    polarity = 0;
    baud_tick_int = 0;
    baud_tick_ts = SC_ZERO_TIME;
    baud_tick_p = SC_ZERO_TIME;
    baud_tick_rate = 0;
    
    // processes
    SC_THREAD(baud_tick_monitor);
    sensitive << baud_tick_int;
  }

  void reset(void) {
    sc_core::sc_time T{100, SC_NS};
    rst_i = 1;
    sc_start(10 * T);
    rst_i = 0;
  }

  void load_baudrate(uint32_t baudrate){
    // baudrate calculation
    bauds_lim = baudrate;
    std::cout << "Baudrate is set to " << baudrate;
  }

  void baud_tick_monitor(){
    while(true)
    {
      if (baud_tick_int)
      {
        if(baud_tick_ts != SC_ZERO_TIME){
          baud_tick_p = sc_time_stamp() - baud_tick_ts;
          baud_tick_rate = 1 / baud_tick_p.to_seconds();
        }
        baud_tick_ts = sc_time_stamp(); // register current pulse timestamp
        /* Calculate, dt and tick rate */
        std:cout << "baud_tick is triggered at " << sc_time_stamp() << endl;
        std::cout << "baud_tick_p : " << baud_tick_p << endl;
        std::cout << "baud_tick_rate : " << (baud_tick_rate/1e6) << "Mhz" << endl;
      }
      wait();
    }
  }
};

#endif // UART_INTF_H