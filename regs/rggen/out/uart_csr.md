## uart_csr

* byte_size
    * 16

|name|offset_address|
|:--|:--|
|[uart_ctrl](#uart_csr-uart_ctrl)|0x0|
|[uart_status](#uart_csr-uart_status)|0x4|
|[uart_rdata](#uart_csr-uart_rdata)|0x8|
|[uart_wdata](#uart_csr-uart_wdata)|0xc|

### <div id="uart_csr-uart_ctrl"></div>uart_ctrl

* offset_address
    * 0x0
* type
    * default
* comment
    * Control register

|name|bit_assignments|type|initial_value|reference|labels|comment|
|:--|:--|:--|:--|:--|:--|:--|
|tx_en|[0]|rw|0x0||||
|rx_en|[1]|rw|0x0||||
|reserved|[15:2]|reserved|0x0000||||
|baud_div|[31:16]|rw|0x0000||||

### <div id="uart_csr-uart_status"></div>uart_status

* offset_address
    * 0x4
* type
    * external
* byte_size
    * 4 bytes
* comment
    * Status register

### <div id="uart_csr-uart_rdata"></div>uart_rdata

* offset_address
    * 0x8
* type
    * default
* comment
    * Read Data Register

|name|bit_assignments|type|initial_value|reference|labels|comment|
|:--|:--|:--|:--|:--|:--|:--|
|rdata|[7:0]|rotrg|0x00||||

### <div id="uart_csr-uart_wdata"></div>uart_wdata

* offset_address
    * 0xc
* type
    * default
* comment
    * Write Data Register

|name|bit_assignments|type|initial_value|reference|labels|comment|
|:--|:--|:--|:--|:--|:--|:--|
|wdata|[7:0]|wotrg|0x00||||
