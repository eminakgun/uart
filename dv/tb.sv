
module tb();

  bit clk;

  always #5 clk = ~clk;

  uart_intf uart_if(clk);

  uart uart(
    .clk_i(clk),
    .rst_i(uart_if.rst_i),
    .rx_i(uart_if.rx_i),
    .tx_data_i(uart_if.tx_data_i),
    .rx_en_i(uart_if.rx_en_i),
    .tx_en_i(uart_if.tx_en_i),
    .parity_en_i(uart_if.parity_en_i),
    .parity_odd_i(uart_if.parity_odd_i),
    .bauds_lim_i(uart_if.bauds_lim_i),
    .tx_done_o(uart_if.tx_done_o),
    .tx_o(uart_if.tx_o),
    .rx_data_o(uart_if.rx_data_o),
    .rx_done_o(uart_if.rx_done_o),
    .frame_err_o(uart_if.frame_err_o),
    .parity_err_o(uart_if.parity_err_o)
  );


  uart_test test;

  initial begin
    uvm_config_db#(virtual uart_if)::set(null, "*.env.m_uart_agent*", "vif", uart_if);
    $timeformat(-12, 0, " ps", 12);
    run_test();
  end

  
endmodule : tb
