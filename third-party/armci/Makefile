CHAPEL_ROOT = ../..
CHAPEL_SUBDIR = third-party/armci
include $(CHAPEL_ROOT)/make/Makefile.base

default: all

all: FORCE
	cd $(ARMCI_SRC_DIR) && $(MAKE) TARGET=$(ARMCI_TARGET) CC=$(CC) FC=gfortran MSG_COMMS=MPI MPI_INCLUDE=$(CHPL_HOME)/third-party/mpich1/install/$(CHPL_MAKE_TARGET_PLATFORM)/include MPI_LIB=$(CHPL_HOME)/third-party/mpich1/install/$(CHPL_MAKE_TARGET_PLATFORM)/lib

clean: FORCE
	cd $(ARMCI_SRC_DIR) && $(MAKE) clean
	-cd $(ARMCI_SRC_DIR) && rm -r lib/$(ARMCI_TARGET)
	-cd $(ARMCI_SRC_DIR)/src && rm -f $(ARMCI_TARGET).MPI.stamp

FORCE:
