puts "Running wave.tcl"

set uart_sigs [list]
set nfacs [gtkwave::getNumFacs]
puts "Number of Facilities : $nfacs"

# Auto added signals
for {set i 0} {$i < $nfacs } {incr i} {
    set facname [gtkwave::getFacName $i]
    lappend uart_sigs "$facname"
}

puts "All signals : $uart_sigs"

gtkwave::/Edit/Highlight_All
gtkwave::addSignalsFromList $uart_sigs
gtkwave::/Time/Zoom/Zoom_Best_Fit


set format_list [list top.dut.uart.BAUDRATE_CNT_WIDTH\[31:0\] top.dut.uart.bauds_counter\[15:0\] top.dut.uart.bauds_lim_i\[15:0\]]
# set format_list [list]
# lappend format_list "{top.dut.uart.BAUDRATE_CNT_WIDTH\[31:0\]}"
# lappend format_list "{top.dut.uart.bauds_counter\[16:0\]}"
# lappend format_list "{top.dut.uart.bauds_lim\[15:0\]}"
puts "format_list : $format_list"
gtkwave::highlightSignalsFromList $format_list
gtkwave::/Edit/Data_Format/Decimal
gtkwave::/Edit/UnHighlight_All

puts "End of the TCL script for GTKWave"