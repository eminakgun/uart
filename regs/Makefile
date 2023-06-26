
RGGEN = $(PWD)/rggen
RGGEN_OUT_DIR = $(RGGEN)/out

gen_reg:
	rggen -c $(RGGEN)/config.yml -o $(RGGEN_OUT_DIR) $(RGGEN)/uart_csr.yml

clean:
	rm -rf $(RGGEN_OUT_DIR)/*