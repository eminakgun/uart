package uart_csr_ral_pkg;
  import uvm_pkg::*;
  import rggen_ral_pkg::*;
  `include "uvm_macros.svh"
  `include "rggen_ral_macros.svh"
  class uart_ctrl_reg_model extends rggen_ral_reg;
    rand rggen_ral_field tx_en;
    rand rggen_ral_field rx_en;
    rand rggen_ral_field baud_div;
    function new(string name);
      super.new(name, 32, 0);
    endfunction
    function void build();
      `rggen_ral_create_field(tx_en, 0, 1, "RW", 0, 1'h0, 1, -1, "")
      `rggen_ral_create_field(rx_en, 1, 1, "RW", 0, 1'h0, 1, -1, "")
      `rggen_ral_create_field(baud_div, 16, 16, "RW", 0, 16'h0000, 1, -1, "")
    endfunction
  endclass
  class uart_rdata_reg_model extends rggen_ral_reg;
    rand rggen_ral_field rdata;
    function new(string name);
      super.new(name, 32, 0);
    endfunction
    function void build();
      `rggen_ral_create_field(rdata, 0, 8, "RO", 1, 8'h00, 1, -1, "")
    endfunction
  endclass
  class uart_wdata_reg_model extends rggen_ral_reg;
    rand rggen_ral_field wdata;
    function new(string name);
      super.new(name, 32, 0);
    endfunction
    function void build();
      `rggen_ral_create_field(wdata, 0, 8, "WO", 0, 8'h00, 1, -1, "")
    endfunction
  endclass
  class uart_csr_block_model #(
    type UART_STATUS = rggen_ral_block,
    bit INTEGRATE_UART_STATUS = 1
  ) extends rggen_ral_block;
    rand uart_ctrl_reg_model uart_ctrl;
    rand UART_STATUS uart_status;
    rand uart_rdata_reg_model uart_rdata;
    rand uart_wdata_reg_model uart_wdata;
    function new(string name);
      super.new(name, 4, 0);
    endfunction
    function void build();
      `rggen_ral_create_reg(uart_ctrl, '{}, 4'h0, "RW", "g_uart_ctrl.u_register")
      `rggen_ral_create_block(uart_status, 4'h4, this, INTEGRATE_UART_STATUS)
      `rggen_ral_create_reg(uart_rdata, '{}, 4'h8, "RO", "g_uart_rdata.u_register")
      `rggen_ral_create_reg(uart_wdata, '{}, 4'hc, "WO", "g_uart_wdata.u_register")
    endfunction
  endclass
endpackage
