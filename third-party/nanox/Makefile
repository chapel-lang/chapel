ifndef CHPL_MAKE_HOME
export CHPL_MAKE_HOME=$(shell pwd)/../..
endif

export CHPL_TASKS=nanox
CHPL_MAKE_HOST_TARGET = --target
include $(CHPL_MAKE_HOME)/make/Makefile.base

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
	rm -rf $(NANOX_BUILD_DIR)

clobber: FORCE
	rm -rf build install

nanox-config: FORCE
	mkdir -p $(NANOX_BUILD_DIR)
	cd $(NANOX_BUILD_DIR) && $(NANOX_SUBDIR)/configure CC='$(CC)' CXX='$(CXX)' --prefix=$(NANOX_INSTALL_DIR) --with-chapel=$(CHPL_MAKE_HOME)

nanox-build: FORCE
	cd $(NANOX_BUILD_DIR) && $(MAKE)
	cd $(NANOX_BUILD_DIR) && $(MAKE) install

nanox: nanox-config nanox-build

nanox-reconfig:
	cd $(NANOX_SUBDIR) && autoreconf -f -i

nanox-getgit:
	git clone http://pm.bsc.es/git/nanox.git
	mv nanox nanox-git

FORCE:
