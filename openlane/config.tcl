# User config
set ::env(DESIGN_NAME) uart
set ::env(VERILOG_FILES) [glob $::env(PRJ_ROOT)/rtl/*.sv]

# Fill this
set ::env(CLOCK_PERIOD) "10.0"
set ::env(CLOCK_PORT) "clk_i"

set filename $::env(DESIGN_DIR)/$::env(PDK)_$::env(STD_CELL_LIBRARY)_config.tcl
if { [file exists $filename] == 1} {
	source $filename
}

