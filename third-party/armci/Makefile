ARMCI_VERSION=1-3
ARMCI_DIR=armci-$(ARMCI_VERSION)

default: all

all: FORCE
	cd $(ARMCI_DIR) && $(MAKE) NOFORT=1 TARGET=LINUX MSG_COMMS=PVM PVM_INCLUDE=/users/bradc/comm/pvm/pvm3.4.4/include

clean: FORCE
	cd $(ARMCI_DIR) && $(MAKE) clean

FORCE:
