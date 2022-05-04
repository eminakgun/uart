
#include "top.hpp"

/**
 * sc_clock has something special. Segmentation fault occurs if clk is not distributed from main.
 * 
 * */ 

int sc_main(int argc, char** argv) {
  Verilated::commandArgs(argc, argv);
  top* tb = new top{"top"};
  sc_time clk_time(100, SC_NS); // 10Mhz,
  sc_clock clk{"clk", clk_time, 0.5, SC_ZERO_TIME, false};
  tb->map_ports(clk);
  sc_start(1, SC_NS); // This is the point where eleboration run until
  
  std::cout << "Starting sim...\n";
  tb->run();
  std::cout << "Ending sim...";

  // delete tb;
  return 0;
}

