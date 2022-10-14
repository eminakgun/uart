
import uart_agent_pkg::*;

class uart_test extends dv_lib_pkg::dv_base_test#(.ENV_T(uart_env));

  virtual uart_intf uart_if;

  uart_agent m_uart_agent;

  function void build_phase(uvm_phase phase);
    super.build_phase(phase);
    m_uart_agent = uart_agent::type_id::create("m_uart_agent", this);
  endfunction

endclass
