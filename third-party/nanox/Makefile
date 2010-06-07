export CHPL_TASKS=nanox
CHAPEL_ROOT = ../..
CHAPEL_SUBDIR = third-party/nanox
CHPL_MAKE_HOST_TARGET = --target
include $(CHAPEL_ROOT)/make/Makefile.base


#
# set up the directories
#
NANOX_ABS_DIR = $(shell pwd)
NANOX_INSTALL_DIR = $(NANOX_ABS_DIR)/$(NANOX_INSTALL_SUBDIR)
NANOX_BUILD_DIR = $(NANOX_ABS_DIR)/$(NANOX_BUILD_SUBDIR)
NANOX_DIR = $(NANOX_ABS_DIR)

default: all

all: nanox

clean: FORCE
	cd $(NANOX_SUBDIR) && make clean

clobber: FORCE
	rm -r $(NANOX_INSTALL_DIR)


nanox-config: FORCE
	mkdir -p $(NANOX_BUILD_DIR)
	cd $(NANOX_BUILD_DIR) && $(NANOX_SUBDIR)/configure CC='$(CC)' CXX='$(CXX)' --prefix=$(NANOX_INSTALL_DIR)

nanox-build: FORCE
	cd $(NANOX_BUILD_DIR) && $(MAKE)
	cd $(NANOX_BUILD_DIR) && $(MAKE) install

nanox: nanox-config nanox-build

FORCE:
