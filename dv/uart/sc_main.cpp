#include "systemc.h"
#include "top.hpp"


int sc_main(int argc, char** argv) {
  Verilated::commandArgs(argc, argv);
  top* tb_top = new top{"top"};
  
  // std::cout << "clk_time value() " << clk_time.value() << endl;
  // std::cout << "clk_time to_double() " << clk_time.to_double() << endl;
  // std::cout << "clk_time to_default_time_units()"  << clk_time.to_default_time_units() << endl;
  // std::cout << "clk_time to_seconds() " << clk_time.to_seconds() << endl;
  // std::cout << "clk_time Freq " << 1/clk_time.to_seconds() << endl;
  // std::cout << "sc_get_time_resolution() " << sc_core::sc_get_time_resolution() << endl;

  // sc_stop();
  
  std::cout << "Starting sim..." << endl;
  while (!tb_top->end_of_tests.trylock())
  {
    sc_start(1, SC_NS);
  }
  
  // tb_top->end_of_tests.lock();
  // tb_top->run();
  std::cout << "Ending sim at " << sc_time_stamp();

  // delete tb;
  return 0;
}
