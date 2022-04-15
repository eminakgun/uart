
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
  input  data_order, // MSB or LSB first
  input  polarity,
  input  [BAUDRATE_CNT_WIDTH-1:0] bauds_lim,
  
  // outs
  output rd_data_o, 
  output tx_o,
  output rx_valid
);
  parameter BAUDRATE_CNT_WIDTH = 16;

  logic [BAUDRATE_CNT_WIDTH:0] bauds_counter;
  logic baud_tick;

  assign baud_tick = bauds_counter[16];

  always_ff @(posedge clk_i) begin
    if (rst_i) begin
      bauds_counter <= 17'h0;
    end
    else begin
      // $display("counting");
      bauds_counter <= {1'b0, bauds_counter[BAUDRATE_CNT_WIDTH-1:0]} + {1'b0, bauds_lim[BAUDRATE_CNT_WIDTH-1:0]};
    end
  end

  // initial begin
  //   $display("Finished");
  //   $finish();
  // end
      
endmodule