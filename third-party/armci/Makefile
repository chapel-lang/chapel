CHAPEL_ROOT = ../..
CHAPEL_SUBDIR = third-party/armci
include $(CHAPEL_ROOT)/make/Makefile.base

ARMCI_VERSION=GPCsockets
ARMCI_DIR=armci-$(ARMCI_VERSION)

default: all

all: FORCE
	cd $(ARMCI_DIR) && $(MAKE) TARGET=LINUX FC=gfortran MSG_COMMS=MPI MPI_INCLUDE=$(CHPL_HOME)/third-party/mpich1/install/$(CHPL_MAKE_PLATFORM)/include MPI_LIB=$(CHPL_HOME)/third-party/mpich1/install/$(CHPL_MAKE_PLATFORM)/lib

clean: FORCE
	cd $(ARMCI_DIR) && $(MAKE) clean
	-cd $(ARMCI_DIR) && rm -r lib
	-cd $(ARMCI_DIR)/src && rm LINUX.MPI.stamp

FORCE:
