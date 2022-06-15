RTL_DIR = $(PWD)/../rtl
TOP_MODULE = uart
SV_SOURCES = $(RTL_DIR)/uart_tx.sv $(RTL_DIR)/uart_rx.sv $(RTL_DIR)/uart.sv
PART ?= xc7z010clg400-1
OUT_DIR = $(PWD)/reports

export

initialize:
	mkdir -p reports

com:
	xvlog -sv $(RTL_DIR)/*

elab: com
	xelab -top $(TOP_MODULE) -snapshot uart_snapshot

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
