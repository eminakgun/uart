
OUT_DIR = $(PWD)/obj_dir
TOP_MODULE = uart

verilate:
	verilator -sc --exe --trace --build $(TOP_MODULE).sv sc_main.cpp

sim:
	$(OUT_DIR)/V$(TOP_MODULE)

wave:
	gtkwave logs/uart_dump.vcd --wish -T wave.tcl

all: verilate sim

clean:
	rm -rf $(OUT_DIR)