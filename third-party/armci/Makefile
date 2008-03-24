CHAPEL_ROOT = ../..
CHAPEL_SUBDIR = third-party/armci
include $(CHAPEL_ROOT)/make/Makefile.base

ARMCI_VERSION=1-3
ARMCI_DIR=armci-$(ARMCI_VERSION)

default: all

all: FORCE
	cd $(ARMCI_DIR) && $(MAKE) NOFORT=1 TARGET=LINUX MSG_COMMS=MPI MPI_INCLUDE=$(CHPL_HOME)/third-party/mpich1/install/$(CHPL_MAKE_PLATFORM)/include MPI_LIB=$(CHPL_HOME)/third-party/mpich1/install/$(CHPL_MAKE_PLATFORM)/lib

clean: FORCE
	cd $(ARMCI_DIR) && $(MAKE) clean

FORCE:
