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

cleanall: FORCE
	rm -rf build

clobber: FORCE
	rm -rf build install


massivethreads-config: FORCE
	cd $(MASSIVETHREADS_SUBDIR) && $(MASSIVETHREADS_SUBDIR)/configure CC='$(CC)' CXX='$(CXX)' --prefix=$(MASSIVETHREADS_INSTALL_DIR)

massivethreads-build: FORCE
	cd $(MASSIVETHREADS_SUBDIR) && $(MAKE)
	cd $(MASSIVETHREADS_SUBDIR) && $(MAKE) install

massivethreads: massivethreads-config massivethreads-build

FORCE:
