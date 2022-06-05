source $::env(PRJ_ROOT)/openlane/prep.tcl
run_synthesis
# this tcl proc calls,
# run_yosys
# run_sta
# simple_cts, optional
# check_assign_statements, optional
# check_synthesis_failure, optional
# Creates a netlist with power/ground pins if SYNTH_USE_PG_PINS_DEFINES is defined
