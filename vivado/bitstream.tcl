   open_checkpoint $::env(OUT_DIR)/place.dcp
   
  # generate a bitstream
  write_bitstream -force $::env(OUT_DIR)/$::env(TOP_MODULE).bit