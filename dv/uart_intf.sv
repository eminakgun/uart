interface uart_intf(input bit clk_i);
  
  logic        rst_i;
  logic        rx_i;      
  logic [7:0]  tx_data_i;
  logic        rx_en_i;
  logic        tx_en_i;
  logic        parity_en_i;
  logic        parity_odd_i;
  logic [15:0] bauds_lim_i;
  logic        tx_done_o;
  logic        tx_o;     
  logic [7:0]  rx_data_o; 
  logic        rx_done_o;
  logic        frame_err_o;
  logic        parity_err_o;


  task reset();
    rst_i <= 1;
    $urandom_range(1, 10);
    rst_i <= 0;
  endtask

  task initialize();
    rx_i <= '0;
    tx_data_i <= '0;
    rx_en_i <= '0;
    tx_en_i <= '0;
    parity_en_i <= '0;
    parity_odd_i <= '0;
    bauds_lim_i <= '0;
  endtask


endinterface 