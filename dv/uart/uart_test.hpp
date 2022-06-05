#ifndef UART_TEST_H
#define UART_TEST_H

#include "systemc.h"
#include "uart_intf.hpp"
#include "globals.hpp"

SC_MODULE(uart_test) {

    SC_CTOR(uart_test){} 

    void run() {
      unsigned int test_size;
      unsigned int baudrate_table[] = {
        110, 300, 600, 1200, 2400, 4800, 9600, 
        14400, 19200, 38400, 57600, 115200, 128000, 230400, 256000,
        460800, 576000, 896000, 921600
      };
      uint8_t tx_rand;
      srand((unsigned int)time(NULL)); // seed rng
      LOG("Start UART Tests");
      uart_if->wait_periods(2000);

      test_size = *(&baudrate_table + 1) - baudrate_table;
      LOG("Test size : " + std::to_string(test_size));
      for (uint i = 3; i < test_size; i++) {
        LOG("Running test: " + std::to_string(i));
        LOG("Testing baudrate :" + std::to_string(baudrate_table[i]));
        uart_if->load_baudrate(baudrate_table[i]);
        tx_rand = rand() % 256;
        uart_if->tx_data_i = tx_rand;
        uart_if->tx_en_i = 1;
        LOG("Transmitting data :" + std::to_string(tx_rand));
        uart_if->parity_en_i  = rand() % 2;
        uart_if->parity_odd_i = rand() % 2;
        wait(uart_if->tx_done_o.posedge_event());
        uart_if->tx_en_i = 0;
        uart_if->wait_periods(200);
      }

      LOG("End UART Tests");
    }

};

#endif // UART_TEST_H