#include "Vuart.h"

#if VM_TRACE
#include <verilated_vcd_sc.h>
#endif


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
  }

  void reset(void) {
    sc_core::sc_time T{100, SC_NS};
    rst_i = 1;
    sc_start(10 * T);
    rst_i = 0;
  }

  void load_baudrate(){
    bauds_lim = 1500;
  }
};


int sc_main(int argc, char** argv) {
  Verilated::commandArgs(argc, argv);

  // sc_trace_file *uart_trace_file;
  // uart_trace_file = sc_create_vcd_trace_file("uart_trace_file");

  int sim_time = 0;
  sc_clock clk{"clk", 100, SC_NS, 0.5, 0, SC_NS, false}; // 10Mhz, negedge first
  Vuart* top = new Vuart{"top"};
  uart_intf* uart_if = new uart_intf("uart_intf");
  // positional port mapping
  top->clk_i(clk);
  top->rst_i(uart_if->rst_i);
  top->rx_i(uart_if->rx_i);
  top->wr_data_i(uart_if->wr_data_i);
  top->rx_en(uart_if->rx_en);
  top->tx_en(uart_if->tx_en);
  top->parity_en(uart_if->parity_en);
  top->parity_odd(uart_if->parity_odd);
  top->data_order(uart_if->data_order);
  top->polarity(uart_if->polarity);
  top->rd_data_o(uart_if->rd_data_o);
  top->tx_o(uart_if->tx_o);
  top->rx_valid(uart_if->rx_valid);
  top->bauds_lim(uart_if->bauds_lim);
  sc_start(1, SC_NS);

  // #if VM_TRACE
  Verilated::mkdir("logs");
  VerilatedVcdSc* trace_f = new VerilatedVcdSc;
  Verilated::traceEverOn(true);
  top->trace(trace_f, 1);
  trace_f->open("logs/uart_dump.vcd");
  // #endif
  
  // sc_trace(uart_trace_file, top->clk_i, "clk");

  uart_if->reset();
  uart_if->load_baudrate();
  // while (!Verilated::gotFinish()) { sc_start(1, SC_NS); }
  while (sim_time < 10000) { sc_start(1, SC_NS); sim_time++;}
  
  // Final model cleanup
  top->final();

  if(trace_f) {
    trace_f->flush();
    trace_f->close();
    trace_f = nullptr;
  }

  // sc_close_vcd_trace_file(uart_trace_file);

  delete top;
  return 0;
}

