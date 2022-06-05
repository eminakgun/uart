
#ifndef GLOBALS_H
#define GLOBALS_H

#include <iostream>

void print_line(std::string str) {
  std::cout << str << " at " << sc_time_stamp() << endl;
}

void log_debug(const char* file, int line , const char* func, const std::string& msg){
   std::cout << "[" << file << "] " << "[" << line << "] " << "[" << func << "] " 
              << msg << " at " << sc_time_stamp() << endl;
}

#define LOG(msg) log_debug(__FILE__, __LINE__, __FUNCTION__, msg)

#include "Vuart.h"
#include "uart_intf.hpp"

extern Vuart* dut_global;
extern uart_intf* uart_if;
extern sc_clock* clk;

sc_time time_100ns(100, SC_NS); // 10Mhz,
sc_clock clk_10mhz{"clk", time_100ns, 0.5, SC_ZERO_TIME, false};
sc_clock clk_20mhz{"clk20", sc_time(50, SC_NS), 0.5, SC_ZERO_TIME, false};
sc_clock clk_50mhz{"clk50", sc_time(20, SC_NS), 0.5, SC_ZERO_TIME, false};
sc_clock clk_100mhz{"clk100", sc_time(10, SC_NS), 0.5, SC_ZERO_TIME, false};

#endif