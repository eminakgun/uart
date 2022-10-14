
module uart (
  input       clk_i,
  input       rst_i,
  input       rx_i,      // rx line
  input [7:0] tx_data_i, // data to be transmitted

  // config
  input        rx_en_i,
  input        tx_en_i,
  input        parity_en_i,
  input        parity_odd_i,
  input [15:0] bauds_lim_i,
  
  // outs
  output       tx_done_o,
  output       tx_o,      // tx line
  output [7:0] rx_data_o, // received data through rx line
  output       rx_done_o,
  output       frame_err_o,
  output       parity_err_o
);

  logic [15:0] bauds_counter;
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

  logic rx_sync1, rx_sync2;
  always_ff @(posedge clk_i) begin : RX_SYNC_FLOPS_2
    rx_sync1 <= rx_i;
    rx_sync2 <= rx_sync1;
  end : RX_SYNC_FLOPS_2

  logic rx_q, rx_q1, rx_q2, rx_filt;
  always_ff @(posedge clk_i) begin : RX_MAJ_VOTING
    rx_q  <= rx_sync2;
    rx_q1 <= rx_q;
    rx_q2 <= rx_q1;
    rx_filt <= (rx_q2 & rx_q1) | (rx_q1 & rx_q) | (rx_q & rx_q2);
  end : RX_MAJ_VOTING

  uart_rx u_uart_rx(
    .clk_i(clk_i),
    .rst_i(rst_i),
    .baud_tick_i(baud_tick_x16),
    .rx_i(rx_filt),
    .parity_en_i(parity_en_i),
    .parity_odd_i(parity_odd_i),
    .rx_en_i(rx_en_i),
    .rx_data_o(rx_data_o),
    .rx_done_o(rx_done_o),
    .frame_err_o(frame_err_o),
    .parity_err_o(parity_err_o)
  );

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