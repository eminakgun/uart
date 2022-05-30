#ifndef UART_TEST_H
#define UART_TEST_H

#include "systemc.h"
#include "uart_intf.hpp"

SC_MODULE(uart_test) {
  public:

    uart_intf* uart_if;

    SC_HAS_PROCESS(uart_test);
    uart_test(sc_module_name n, uart_intf* uart_if) : sc_module(n) 
    {
      this->uart_if = uart_if;
      // SC_THREAD(run);
      //sensitive << this->uart_if->clk_i.posedge_event();
    } 

    void run() {
      unsigned int test_size;
      unsigned int baudrate_table[] = {
        110, 300, 600, 1200, 2400, 4800, 9600, 
        14400, 19200, 38400, 57600, 115200, 128000, 230400, 256000,
        460800, 576000, 896000, 921600
      };
      uint8_t tx_rand;

      std::cout << "Start UART Tests" << endl;
      uart_if->wait_periods(2000);
      // wait(clk_time*20);

      test_size = *(&baudrate_table + 1) - baudrate_table;
      std::cout << "test_size: " << test_size << endl;
      for (uint i = 3; i < test_size; i++) {
        LOG("Running test: " + std::to_string(i));
        std::cout << "Testing baudrate : " <<  baudrate_table[i] << endl;
        uart_if->load_baudrate(baudrate_table[i]);
        // uart_if->load_baudrate(576000);
        uart_if->tx_en_i = 1;
        tx_rand = rand() % 256;
        uart_if->tx_data_i = tx_rand;
        std::cout << "Transmitting data : " << std::to_string(tx_rand) << endl;
        uart_if->parity_en_i  = rand() % 2;
        uart_if->parity_odd_i = rand() % 2;
        wait(uart_if->tx_done_o.posedge_event());
        uart_if->tx_en_i = 0;
        uart_if->wait_periods(200);
        // sc_stop();
      }

      // std::cout << "Testing 9600 baudrate" << endl;
      // // test 9600 
      // uart_if->load_baudrate(9600);
      // uart_if->tx_en_i = 1;
      // uart_if->tx_data_i = 0xAA; // h'AA
      // wait(uart_if->tx_done_o.posedge_event());

      // // test 115200 
      // std::cout << "Testing 115200 baudrate" << endl;
      // uart_if->tx_en_i = 0;
      // // wait(clk_time*20);
      // uart_if->wait_periods(20);
      // uart_if->load_baudrate(115200);
      // uart_if->tx_en_i = 1;
      // wait(uart_if->tx_done_o.posedge_event());
      
      // // test 921600 
      // uart_if->tx_en_i = 0;
      // // wait(clk_time*20);
      // uart_if->wait_periods(20);
      // uart_if->load_baudrate(921600);
      // uart_if->tx_en_i = 1;
      // wait(uart_if->tx_done_o.posedge_event());

      std::cout << "End UART Tests" << endl;
    }

};

#endif // UART_TEST_H