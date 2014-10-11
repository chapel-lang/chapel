ifndef CHPL_MAKE_HOME
export CHPL_MAKE_HOME=$(shell pwd)/../..
endif

CHPL_MAKE_HOST_TARGET = --target
include $(CHPL_MAKE_HOME)/make/Makefile.base

#
# set up the directories
#
MASSIVETHREADS_ABS_DIR = $(shell pwd)
MASSIVETHREADS_INSTALL_DIR = $(MASSIVETHREADS_ABS_DIR)/$(MASSIVETHREADS_INSTALL_SUBDIR)
MASSIVETHREADS_BUILD_DIR = $(MASSIVETHREADS_ABS_DIR)/$(MASSIVETHREADS_BUILD_SUBDIR)
MASSIVETHREADS_DIR = $(MASSIVETHREADS_ABS_DIR)

default: all

all: massivethreads

clean: FORCE
	rm -rf $(MASSIVETHREADS_BUILD_SUBDIR)

cleanall: FORCE
	rm -rf build

clobber: FORCE
	rm -rf build install


massivethreads-config: FORCE
	mkdir -p $(MASSIVETHREADS_BUILD_DIR)
	cd $(MASSIVETHREADS_BUILD_DIR) && $(MASSIVETHREADS_SUBDIR)/configure CC='$(CC)' CXX='$(CXX)' --prefix=$(MASSIVETHREADS_INSTALL_DIR) --enable-malloc-wrapper=no --enable-eco-mode=no --enable-bind-workers=no

massivethreads-build: FORCE
	cd $(MASSIVETHREADS_BUILD_DIR) && $(MAKE)
	cd $(MASSIVETHREADS_BUILD_DIR) && $(MAKE) install

massivethreads: massivethreads-config massivethreads-build

FORCE:

.NOTPARALLEL:
