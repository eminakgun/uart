
#ifndef TOP_H
#define TOP_H

#include "systemc.h"
#include "verilated_sc.h"
#include "verilated.h"

#if VM_TRACE
#include <verilated_vcd_sc.h>
#endif

#include "Vuart.h"
#include "Vuart_uart.h"
#include "Vuart_uart_tx.h"
#include "uart_intf.hpp"
#include "uart_test.hpp"

#include "globals.hpp"

/**
 * sc_clock has something special. Segmentation fault occurs if clk is not distributed from main.
 * 
 * */ 

sc_time time_100ns(100, SC_NS); // 10Mhz,
sc_clock clk_10mhz{"clk", time_100ns, 0.5, SC_ZERO_TIME, false};
sc_clock clk_20mhz{"clk20", sc_time(50, SC_NS), 0.5, SC_ZERO_TIME, false};
sc_clock clk_50mhz{"clk50", sc_time(20, SC_NS), 0.5, SC_ZERO_TIME, false};
sc_clock clk_100mhz{"clk100", sc_time(10, SC_NS), 0.5, SC_ZERO_TIME, false};

SC_MODULE(top) {
  public:
    
    Vuart* dut;
    uart_intf* uart_if;
    uart_test* test;
    sc_mutex end_of_tests;
    sc_clock* clk;

    SC_CTOR(top) {
      clk = &clk_100mhz;
      dut = new Vuart{"dut"};
      uart_if = new uart_intf{"uart_intf", clk->period()};
      test = new uart_test{"uart_test", uart_if};
      end_of_tests.lock();

      map_ports();
      // sc_start(1, SC_NS); // This is the point where eleboration run until

      SC_THREAD(read_internals);
      sensitive << dut->clk_i.pos();

      SC_THREAD(run);
    }

    void map_ports(){
      // positional port mapping
      dut->clk_i(*clk);
      uart_if->clk_i(*clk);
      dut->rst_i(uart_if->rst_i);
      dut->bauds_lim_i(uart_if->bauds_lim_i);
      dut->rx_i(uart_if->rx_i);
      dut->tx_data_i(uart_if->tx_data_i);
      dut->rx_en_i(uart_if->rx_en_i);
      dut->tx_en_i(uart_if->tx_en_i);
      dut->parity_en_i(uart_if->parity_en_i);
      dut->parity_odd_i(uart_if->parity_odd_i);
      dut->tx_done_o(uart_if->tx_done_o);
      dut->tx_o(uart_if->tx_o);
      dut->rx_data_o(uart_if->rx_data_o);
      dut->rx_valid_o(uart_if->rx_valid_o);
    }

    void read_internals(){
      while(true){
        // cout << "Target awoke at " << sc_time_stamp() << endl;
        uart_if->baud_tick_x16 = dut->uart->baud_tick_x16;
        uart_if->baud_tick = dut->uart->uart_tx->baud_tick;
        wait();
      }
    }

    void run(){
      int sim_time = 0;

      // #if VM_TRACE
      Verilated::mkdir("logs");
      VerilatedVcdSc* trace_f = new VerilatedVcdSc;
      Verilated::traceEverOn(true);
      dut->trace(trace_f, 1);
      trace_f->open("sim/uart_dump.vcd");
      // #endif

      std::cout <<  "Clock frequency : " <<  1/clk->period().to_seconds()/1e6 << "mHz" << endl;
      uart_if->reset();

      // while (!Verilated::gotFinish()) { sc_start(1, SC_NS); }
      // while (sim_time < 10000) { 
      //   sc_start(clk_time); // run 1 clk period 
      //   sim_time++;
      // }

      // sc_start(10, SC_MS);
      test->run();
      
      // Final model cleanup
      dut->final();

      if(trace_f) {
        trace_f->flush();
        trace_f->close();
        trace_f = nullptr;
      }

      // sc_close_vcd_trace_file(uart_trace_file);
      delete uart_if;
      delete dut;

      end_of_tests.unlock();
    }
};

#endif // TOP_H