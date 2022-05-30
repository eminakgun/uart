
module uart(
  input  clk_i,
  input  rst_i,
  input  rx_i, // rx line
  input [7:0] tx_data_i, // data to be transmitted

  // config
  input  rx_en_i,
  input  tx_en_i,
  input  parity_en_i,
  input  parity_odd_i,
  input  [BAUDRATE_CNT_WIDTH-1:0] bauds_lim_i,
  
  // outs
  output tx_done_o,
  output tx_o, // tx line
  output [7:0] rx_data_o, // received data through rx line
  output rx_valid_o
);
  parameter BAUDRATE_CNT_WIDTH = 16;

  logic [BAUDRATE_CNT_WIDTH-1:0] bauds_counter;
  logic baud_tick_x16; // x16 faster than baudrate

  /* Baudrate tick generation */
  assign baud_tick_x16 = (bauds_counter == bauds_lim_i) ? 1'b1 : 1'b0;
  always_ff @(posedge clk_i) begin : BAUD_GEN
    if (rst_i) begin
      bauds_counter <= 16'h0;
    end
    else if(tx_en_i || rx_en_i) begin
      if(bauds_counter == bauds_lim_i)
        bauds_counter <= '0;
      else
        bauds_counter <= bauds_counter + 1'b1;
    end
  end : BAUD_GEN

  // uart_rx u_uart_rx(
  //   .rx_i(rx_i),
  //   .baud_tick_i(baud_tick_x16)
  // );

  uart_tx uart_tx(
    .clk_i(clk_i),
    .rst_i(rst_i),
    .baud_tick_x16_i(baud_tick_x16),
    .parity_en_i(parity_en_i),
    .parity_odd_i(parity_odd_i),
    .tx_en_i(tx_en_i),
    .tx_data_i(tx_data_i),
    .tx_done_o(tx_done_o),
    .tx_o(tx_o)
  );

      
endmodule