ifndef CHPL_MAKE_HOME
export CHPL_MAKE_HOME=$(shell pwd)/../..
endif

CHPL_MAKE_HOST_TARGET = --target
include $(CHPL_MAKE_HOME)/make/Makefile.base

#
# set up the directories
#
GMP_ABS_DIR = $(shell pwd)
GMP_INSTALL_DIR = $(GMP_ABS_DIR)/$(GMP_INSTALL_SUBDIR)
GMP_BUILD_DIR = $(GMP_ABS_DIR)/$(GMP_BUILD_SUBDIR)
GMP_DIR = $(GMP_ABS_DIR)

default: all

all: gmp

clean: FORCE
	rm -rf $(GMP_BUILD_SUBDIR)

cleanall: FORCE
	rm -rf build

clobber: FORCE
	rm -rf build install

gmp-config: FORCE
	mkdir -p $(GMP_BUILD_DIR)
	cd $(GMP_BUILD_DIR) && $(GMP_SUBDIR)/configure CC='$(CC)' CXX='$(CXX)' --prefix=$(GMP_INSTALL_DIR)

gmp-build: FORCE
	cd $(GMP_BUILD_DIR) && $(MAKE)
	cd $(GMP_BUILD_DIR) && $(MAKE) check
	cd $(GMP_BUILD_DIR) && $(MAKE) install

gmp: gmp-config gmp-build

FORCE:
