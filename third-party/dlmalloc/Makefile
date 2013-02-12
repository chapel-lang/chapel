ifndef CHPL_MAKE_HOME
export CHPL_MAKE_HOME=$(shell pwd)/../..
endif

CHPL_MAKE_HOST_TARGET = --target
include $(CHPL_MAKE_HOME)/make/Makefile.base

default: all

all: FORCE
	cd src && $(MAKE)

clean: FORCE
	rm -rf $(DLMALLOC_BUILD_SUBDIR)

clobber: FORCE
	rm -rf build install

FORCE:
