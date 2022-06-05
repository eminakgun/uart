
#ifndef TOP_H
#define TOP_H

#include "systemc.h"
#include <ctime>
#include "verilated_sc.h"
#include "verilated.h"

// #if VM_TRACE
#include <verilated_vcd_sc.h>
// #endif

#include "Vuart.h"
#include "Vuart_uart.h"
#include "Vuart_uart_tx.h"
#include "uart_intf.hpp"
#include "uart_test.hpp"

#include "globals.hpp"

SC_MODULE(top) {
  public:
    
    uart_test* test;
    sc_mutex end_of_tests; // don't lock in constructor

    SC_CTOR(top) {
      test = new uart_test("uart_test");
      end_of_tests.lock();

      SC_THREAD(read_internals_thread);
      sensitive << dut_global->clk_i.pos();

      SC_THREAD(run_thread);
    }

    void read_internals_thread(){
      while(true){
        // cout << "Target awoke at " << sc_time_stamp() << endl;
        uart_if->baud_tick_x16 = dut_global->uart->baud_tick_x16;
        uart_if->baud_tick = dut_global->uart->uart_tx->baud_tick;
        wait();
      }
    }

    void run_thread() {
      end_of_tests.lock();

      // #if VM_TRACE
      Verilated::mkdir("logs");
      VerilatedVcdSc* trace_f = new VerilatedVcdSc;
      Verilated::traceEverOn(true);
      dut_global->trace(trace_f, 1);
      trace_f->open("logs/uart_dump.vcd");
      // #endif

      LOG("Clock frequency : " + std::to_string(1/clk->period().to_seconds()/1e6) + "mHz");
      
      uart_if->reset();
      test->run();

      if(trace_f) {
        trace_f->flush();
        trace_f->close();
        // delete trace_f;
        trace_f = nullptr;
        LOG("Shut down trace file");
      }

      end_of_tests.unlock();
      wait(clk->period()*20);
      LOG("Exit run thread...");
    }
};

#endif // TOP_H