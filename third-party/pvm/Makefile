CHAPEL_ROOT = ../..
CHAPEL_SUBDIR = third-party/pvm
include $(CHAPEL_ROOT)/make/Makefile.base

PVM_ABS_DIR = $(shell pwd)
PVM_SUBDIR = pvm-$(PVM_VERSION)/pvm3

default: all

all: pvm

clean: FORCE
	cd $(PVM_SUBDIR) && make clean

clobber: FORCE

pvm: FORCE
	cd $(PVM_SUBDIR) && PVM_ROOT=$(PVM_ABS_DIR)/$(PVM_SUBDIR) && export PVM_ROOT && $(MAKE)

FORCE:
