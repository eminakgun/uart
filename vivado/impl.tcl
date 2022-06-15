 open_checkpoint $::env(OUT_DIR)/synth.dcp
 
 puts "running opt_design"
 # placement
  opt_design
  report_timing_summary -file $::env(OUT_DIR)/opt_timing.rpt
  puts "running place_design"
  place_design
  write_checkpoint -force $::env(OUT_DIR)/place.dcp
  report_clock_utilization -file $::env(OUT_DIR)/clock_util.rpt
  report_utilization -file $::env(OUT_DIR)/place_util.rpt

  # route
  puts "running route_design"
  route_design
  write_checkpoint -force $::env(OUT_DIR)/route.dcp
  report_timing_summary -file $::env(OUT_DIR)/route_timing.rpt
  report_route_status -file $::env(OUT_DIR)/route_status.rpt
  report_power -file $::env(OUT_DIR)/route_power.rpt
  report_drc -file $::env(OUT_DIR)/post_imp_drc.rpt
  write_verilog -force $::env(OUT_DIR)/$::env(TOP_MODULE)_netlist.sv -mode timesim -sdf_anno true