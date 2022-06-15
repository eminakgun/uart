module uart_rx (
  input logic clk_i,
  input logic baud_tick_i,
  input logic rx_i,

  // Outputs
  output logic [7:0] rx_data

);
  
endmodule