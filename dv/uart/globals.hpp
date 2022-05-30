
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

#define LOG(msg) log_debug(__FILE__, __LINE__, __FUNCTION__, msg )

#endif