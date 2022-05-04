
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
#include "uart_intf.hpp"

SC_MODULE(top) {
  public:
    
    Vuart* dut;
    uart_intf* uart_if;
    
    SC_CTOR(top) {
      dut = new Vuart{"dut"};
      uart_if = new uart_intf{"uart_intf"};

      SC_THREAD(read_internals);
      sensitive << dut->clk_i.pos();
    }

    void map_ports(sc_clock& clk){
      // positional port mapping
      dut->clk_i(clk);
      dut->rst_i(uart_if->rst_i);
      dut->rx_i(uart_if->rx_i);
      dut->wr_data_i(uart_if->wr_data_i);
      dut->rx_en(uart_if->rx_en);
      dut->tx_en(uart_if->tx_en);
      dut->parity_en(uart_if->parity_en);
      dut->parity_odd(uart_if->parity_odd);
      dut->data_order(uart_if->data_order);
      dut->polarity(uart_if->polarity);
      dut->rd_data_o(uart_if->rd_data_o);
      dut->tx_o(uart_if->tx_o);
      dut->rx_valid(uart_if->rx_valid);
      dut->bauds_lim(uart_if->bauds_lim);
    }

    void read_internals(){
      while(true){
        // cout << "Target awoke at " << sc_time_stamp() << endl;
        uart_if->baud_tick_int = dut->uart->baud_tick;
        wait();
      }
    }

    void run(){

      int sim_time = 0;
      sc_time clk_time(100, SC_NS);

      // #if VM_TRACE
      Verilated::mkdir("logs");
      VerilatedVcdSc* trace_f = new VerilatedVcdSc;
      Verilated::traceEverOn(true);
      dut->trace(trace_f, 1);
      trace_f->open("logs/uart_dump.vcd");
      // #endif

      uart_if->reset();
      uart_if->load_baudrate(115200);

      // while (!Verilated::gotFinish()) { sc_start(1, SC_NS); }
      while (sim_time < 10000) { 
        sc_start(clk_time); // run 1 clk period 
        // sc_start(1, SC_NS); 
        sim_time++;
      }
      
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
    }
};

#endif // TOP_H