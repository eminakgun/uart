set synth_args ""
set elab ""

foreach ix $argv {
  switch $ix {
    rtl {
      set synth_args "$synth_args -rtl "
      set elab 1
    }
    default {
      puts "Arg does not match any valid switches"
    }
  }
}

puts "synth_args : $synth_args"

read_verilog $::env(SV_SOURCES)
# read_xdc $xdc
puts "running synth_design"
synth_design {*}$synth_args -top $::env(TOP_MODULE) -part $::env(PART)

# open synthesized design
if {$elab == 1} {
  start_gui  
} else {
  report_timing -file $::env(OUT_DIR)/synth_timing.rpt
  report_utilization -file $::env(OUT_DIR)/synth_util.rpt
  write_checkpoint -force $::env(OUT_DIR)/synth.dcp
}
