module uart_rx (
  input logic clk_i,
  input logic rst_i,
  input logic baud_tick_i,
  input logic rx_i,
  input logic parity_en_i,
  input logic parity_odd_i,
  input logic rx_en_i,

  // Outputs
  output logic [7:0] rx_data_o,
  output logic rx_done_o,
  output logic frame_err_o,
  output logic parity_err_o
);
  
  logic [3:0] baud_tick_cnt; //4 bits since we must count to 15 
  logic [3:0] rx_bits_cnt; // max number of bits can be 10
  logic [7:0] sreg; // shift register only for data

  logic rx_start;
  logic baud_tick;
  logic parity_calc;
  logic parity_rx;

  always_ff @(posedge clk_i) begin : BAUD_TICK_COUNTER
    if (rst_i) begin
      baud_tick <= 1'b0;
    end
    else if (rx_en_i && baud_tick_i) begin
      // generate tick when counter overflows
      {baud_tick, baud_tick_cnt} <= {1'b0, baud_tick_cnt} + 5'h1;
    end else begin
      baud_tick <= 1'b0;
    end
  end : BAUD_TICK_COUNTER

  always_ff @(posedge clk_i) begin : RX_LOGIC

    if (rst_i) begin
      sreg <= '0;
      rx_bits_cnt <= '0;
      baud_tick_cnt <= '0;
      rx_data_o <= '0;
      rx_done_o <= 1'b0;
      parity_err_o <= 1'b0;
      frame_err_o <= 1'b0;
    end
    else begin
      if (rx_en_i && rx_i == 0 && rx_start == 0) begin : INITIALIZE_RX
        rx_start <= 1; // start new frame
        rx_bits_cnt <= parity_en_i ? 4'd11 : 4'd10; // include stop bit as well
        baud_tick_cnt <= 7;
        parity_calc <= parity_odd_i; // initial value
      end : INITIALIZE_RX
  
      if (rx_start && baud_tick) begin : RECEIVER
        rx_bits_cnt <= rx_bits_cnt - 1;
        if (parity_en_i) parity_calc <= parity_calc ^ rx_i;

        if (rx_bits_cnt == 0) begin : STOP_BIT
          if (rx_i != 1'b1) frame_err_o <= 1'b1; // this should be stop bit
        end : STOP_BIT
        else if (parity_en_i && rx_bits_cnt ==  1) begin : PARITY_BIT
          parity_rx <= rx_i;
        end 
        else if(rx_bits_cnt == 4'b0) begin : END_OF_XFER
          // end of xfer
          rx_start <= 1'b0;
          if (parity_en_i && parity_rx != parity_calc) begin : PARITY_ERROR
            parity_err_o <= 1;
          end
          else begin : SUCCESS
            rx_done_o <= 1'b1;
            rx_data_o <= sreg;
          end 
        end 
        else begin
          sreg <= {rx_i, sreg[7:1]}; // shift right since lsb first
        end
      end : RECEIVER
    end

  end : RX_LOGIC

endmodule