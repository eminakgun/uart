
module uart(
  input  clk_i,
  input  rst_i,
  input  rx_i,
  input  wr_data_i,

  // config
  input  rx_en,
  input  tx_en,
  input  parity_en,
  input  parity_odd,
  input  [BAUDRATE_CNT_WIDTH-1:0] bauds_lim,
  
  // outs
  output rd_data_o, 
  output tx_o,
  output rx_valid
);
  parameter BAUDRATE_CNT_WIDTH = 16;

  logic [BAUDRATE_CNT_WIDTH-1:0] bauds_counter;
  logic baud_tick_x16; // x16 faster than baudrate

  /* Baudrate tick generation */
  assign baud_tick_x16 = (bauds_counter == bauds_lim) ? 1'b1 : 1'b0;
  always_ff @(posedge clk_i) begin : BAUD_GEN
    if (rst_i) begin
      bauds_counter <= 16'h0;
    end
    else if(tx_en || rx_en) begin
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
    .parity_en_i(uart_csr.parity_en),
    .parity_odd_i(uart_csr.parity_odd),
    .tx_en_i(uart_csr.rx_en),
    .tx_data_i(uart_data.tx_data),
    .tx_o_i(tx_o)
  );

  // registers
  struct packed {
    logic tx_en;
    logic rx_en;
    logic parity_en;
    logic parity_type;
    logic [15:0] baud_div;
    logic system_loopback;
    logic line_loopback;
  } uart_ctrl;

  struct packed {
    logic tx_fifo_full;
    logic tx_fifo_empty;
    logic tx_fifo_overflow;
    logic tx_fifo_reset;
    logic rx_fifo_full;
    logic rx_fifo_empty;
    logic rx_fifo_underflow;
    logic rx_fifo_reset;
  } fifo_csr;

  struct packed {
    logic rx_parity_err;
    logic rx_framing_err;
  } uart_errors;

  struct packed {
    logic [7:0] tx_data;
    logic [7:0] rx_data;
  } uart_data;

  
      
endmodule