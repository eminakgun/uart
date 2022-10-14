DV_DIR = $(PWD)/../dv
RTL_DIR = $(PWD)/../rtl
TOP_MODULE = uart
SV_SOURCES = $(RTL_DIR)/uart_tx.sv $(RTL_DIR)/uart_rx.sv $(RTL_DIR)/uart.sv
DV_SOURCES = $(DV_DIR)/uart_intf.sv $(DV_DIR)/uart_test.sv $(DV_DIR)/tb.sv 
TB_TOP = tb
PART ?= xc7z010clg400-1
OUT_DIR = $(PWD)/reports
OT_HW_DV = $(OPENTITAN_PATH)/hw/dv/sv
OT_INCLUDE = $(OPENTITAN_PATH)/hw/dv/sv/dv_utils
OT_ARGS = -i $(OT_INCLUDE) -d UVM

export

initialize:
	mkdir -p reports

com_dv_lib:
	xvlog -L uvm -sv $(OPENTITAN_PATH)/hw/top_earlgrey/rtl/top_pkg.sv $(OT_ARGS)
	xvlog -L uvm -sv $(OT_HW_DV)/bus_params_pkg/bus_params_pkg.sv $(OT_ARGS)
	xvlog -L uvm -sv $(OT_HW_DV)/dv_utils/dv_test_status_pkg.sv $(OT_ARGS)
	xvlog -L uvm -sv $(OT_HW_DV)/dv_utils/dv_utils_pkg.sv $(OT_ARGS)
	xvlog -L uvm -sv $(OT_HW_DV)/str_utils/str_utils_pkg.sv $(OT_ARGS)
	xvlog -L uvm -sv $(OT_HW_DV)/dv_base_reg/dv_base_reg_pkg.sv $(OT_ARGS)
	xvlog -L uvm -sv $(OT_HW_DV)/csr_utils/csr_utils_pkg.sv $(OT_ARGS)
	xvlog -L uvm -sv $(OT_HW_DV)/dv_lib/dv_lib_pkg.sv $(OT_ARGS)

com_uart_agent:
	xvlog -L uvm -sv $(OT_HW_DV)/uart_agent/uart_agent_pkg.sv $(OT_ARGS) -i $(OT_HW_DV)/uart_agent -i $(OT_HW_DV)/uart_agent/seq_lib

com_dv: com_rtl
	xvlog -L uvm -sv $(DV_SOURCES)

sim:
	xsim

com_rtl:
	xvlog -sv $(RTL_DIR)/*

elab: com_dv
	xelab -top $(TB_TOP) -snapshot uart_snapshot

elab_rtl:
	vivado -mode tcl -source synth.tcl -notrace -tclargs rtl

synth: initialize
	vivado -mode tcl -source synth.tcl -notrace

impl: $(OUT_DIR) $(OUT_DIR)/synth.dcp
	vivado -mode tcl -source impl.tcl -notrace

bit: $(OUT_DIR) $(OUT_DIR)/route.dcp
	vivado -mode tcl -source bitstream.tcl -notrace

vivado:
	vivado -mode tcl

all: initialize
	vivado -mode batch -source synth.tcl impl.tcl bitstream.tcl -notrace > $(OUT_DIR)/std_out.log

clean:
	rm -rf *.jou *.log *.pb *.str xsim.dir .Xil $(OUT_DIR)
