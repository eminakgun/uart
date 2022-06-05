
PRJ_ROOT = $(PWD)/..
OPENLANE_TAG ?= 2021.11.23_01.42.34
OPENLANE_IMAGE_NAME ?= efabless/openlane:$(OPENLANE_TAG)
DOCKER_OPTIONS = --ipc=host 
DOCKER_OPTIONS += -e DISPLAY=$(DISPLAY) -v /tmp/.X11-unix:/tmp/.X11-unix -v $(HOME)/.Xauthority:/.Xauthority --network host
DOCKER_CMD = 	docker run -it --rm \
							-v $(PDK_ROOT):$(PDK_ROOT) \
							-v $(PRJ_ROOT):$(PRJ_ROOT) \
							-e PDK_ROOT=$(PDK_ROOT) \
							-e PRJ_ROOT=$(PRJ_ROOT) \
							-u $(shell id -u $(USER)):$(shell id -g $(USER)) \
							$(DOCKER_OPTIONS) $(OPENLANE_IMAGE_NAME)

ol_docker:
	$(DOCKER_CMD)

# run openlane interactiely
ol_it:
	$(DOCKER_CMD) sh -c "flow.tcl -it -file prep.tcl"

# generate initial files
ol_prep:
	$(DOCKER_CMD) sh -c "flow.tcl -design $(PRJ_ROOT)/openlane -init_design_config"

synth:
	$(DOCKER_CMD) sh -c \
	"flow.tcl -interactive -file $(PRJ_ROOT)/openlane/synth.tcl"

yosys:
	$(DOCKER_CMD) sh -c \
	"flow.tcl -interactive -file $(PRJ_ROOT)/openlane/yosys.tcl"

clean:
	rm -rf src runs




