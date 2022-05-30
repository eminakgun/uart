
`ifndef UART_REG_H
`define UART_REG_H

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

`endif