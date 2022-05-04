
module uart_tx (
  input logic clk_i,
  input logic rst_i,
  input logic baud_tick_x16_i,
  input logic parity_en_i,
  input logic parity_odd_i,
  input logic tx_en_i,
  input logic [7:0] tx_data_i,
  //Outputs
  output logic tx_o
);

logic parity;
logic tx_start;
logic baud_tick;
logic [3:0] baud_tick_cnt; 
logic [3:0] tx_bits_cnt; // max number of bits can be 10
logic [10:0] sreg; // shift register including (start + data + parity + stop)


assign tx_o = (tx_start) ? sreg[0] : 1'b1;
assign parity = (^tx_data_i) ^ parity_odd_i; // calculate parity

always_ff @(posedge clk_i) begin : baud_tick_counter
  if (rst_i) begin
    baud_tick <= 1'b0;
  end
  else if (baud_tick_x16_i) begin
    // generate tick when counter overflows
    {baud_tick, baud_tick_cnt} <= {1'b0, baud_tick_cnt} + 5'h1;
  end else begin
    baud_tick <= 1'b0;
  end
end : baud_tick_counter

always_ff @(posedge clk_i) begin : tx_shift_register
  if (rst_i) begin
    tx_bits_cnt <= 4'b0;
  end
  else begin
    if (tx_en_i || !tx_start) begin : initialize
      // enabled but not yet started transmitting
      sreg <= {1'b1, (parity_en_i ? parity : 1'b1), tx_data_i, 1'b0};
      tx_bits_cnt <= parity_en_i ? 4'd11 : 4'd10;
      tx_start <= 1'b1;
    end : initialize
    if (baud_tick || tx_start) begin : transmitter
      sreg <= {1'b1, sreg[10:1]}; // padd left with 1's
      tx_bits_cnt <= tx_bits_cnt - 1'b1;
      if(tx_bits_cnt == 4'b0) begin
        // end of xfer
        tx_start <= 1'b0;
      end 
    end : transmitter
  end
end : tx_shift_register
  
endmodule