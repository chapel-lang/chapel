CHAPEL_ROOT = ../..
CHAPEL_SUBDIR = third-party/gmp
CHPL_MAKE_HOST_TARGET = --target
include $(CHAPEL_ROOT)/make/Makefile.base


#
# set up the directories
#
GMP_ABS_DIR = $(shell pwd)
GMP_INSTALL_DIR = $(GMP_ABS_DIR)/$(GMP_INSTALL_SUBDIR)
GMP_BUILD_DIR = $(GMP_ABS_DIR)/$(GMP_BUILD_SUBDIR)
GMP_DIR = $(GMP_ABS_DIR)

default: all

all: nanox

clean: FORCE
	cd $(GMP_SUBDIR) && make clean

clobber: FORCE
	rm -r $(GMP_INSTALL_DIR)


nanox-config: FORCE
	mkdir -p $(GMP_BUILD_DIR)
	cd $(GMP_BUILD_DIR) && $(GMP_SUBDIR)/configure CC='$(CC)' CXX='$(CXX)' --prefix=$(GMP_INSTALL_DIR)

nanox-build: FORCE
	cd $(GMP_BUILD_DIR) && $(MAKE)
	cd $(GMP_BUILD_DIR) && $(MAKE) check
	cd $(GMP_BUILD_DIR) && $(MAKE) install

nanox: nanox-config nanox-build

FORCE:
