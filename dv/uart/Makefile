
OUT_DIR = $(PWD)/verilated
RTL_DIR = $(PWD)/../../rtl
TOP_MODULE = uart

verilate:
	verilator --sc --trace --exe --build +incdir+$(RTL_DIR) --Mdir $(OUT_DIR) \
		$(RTL_DIR)/$(TOP_MODULE).sv sc_main.cpp sim/uart_cfg.vlt

.PHONY: sim
sim:
	$(OUT_DIR)/V$(TOP_MODULE)

wave:
	gtkwave sim/uart_dump.vcd --wish -T sim/wave.tcl

all: clean verilate sim

clean:
	rm -rf $(OUT_DIR) $(PWD)/sim/uart_dump.vcd