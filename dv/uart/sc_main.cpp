#include "systemc.h"
#include "Vuart.h"
#include "globals.hpp"
#include "top.hpp"
#include "uart_intf.hpp"

// global variable declerations
sc_clock* clk = &clk_100mhz; 
Vuart* dut_global; // pointer to dut object
uart_intf* uart_if = new uart_intf{"uart_intf", clk->period()};

/** Lessons Learned
 * sc_clock has something special. Segmentation fault occurs if clk is not distributed from main.
 * DUT object has to be constructed in sc_main, not in global scope, otherwise segmentation fault occurs
*/ 


int sc_main(int argc, char** argv) {
  Verilated::commandArgs(argc, argv);
  
  
  Vuart dut("dut"); 
  dut_global = &dut; // update pointer variable

  // bind dut and intferface ports
  uart_if->clk_i(*clk);
  dut.clk_i(*clk);
  dut.rst_i(uart_if->rst_i);
  dut.bauds_lim_i(uart_if->bauds_lim_i);
  dut.rx_i(uart_if->rx_i);
  dut.tx_data_i(uart_if->tx_data_i);
  dut.rx_en_i(uart_if->rx_en_i);
  dut.tx_en_i(uart_if->tx_en_i);
  dut.parity_en_i(uart_if->parity_en_i);
  dut.parity_odd_i(uart_if->parity_odd_i);
  dut.tx_done_o(uart_if->tx_done_o);
  dut.tx_o(uart_if->tx_o);
  dut.rx_data_o(uart_if->rx_data_o);
  dut.rx_valid_o(uart_if->rx_valid_o);

  // create top testbench 
  top* tb_top = new top{"top"};
  
  LOG("Starting sim");
  while (true)
  {
    sc_start(1, SC_NS);
    if (tb_top->end_of_tests.trylock() == 0) {
      LOG("Reached end of tests");
      break;
    }
  }
  
  // Final model cleanup
  dut.final();
  LOG("Ending sim at ");
  delete tb_top;
  delete uart_if;
  return 0;
}
