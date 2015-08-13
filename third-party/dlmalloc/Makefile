ifndef CHPL_MAKE_HOME
export CHPL_MAKE_HOME=$(shell pwd)/../..
endif

CHPL_MAKE_HOST_TARGET = --target
include $(CHPL_MAKE_HOME)/make/Makefile.base

ifeq ($(CHPL_MAKE_TARGET_ARCH),knc)
ifeq ($(CHPL_MAKE_TARGET_COMPILER),intel)
CHPL_DLMALLOC_CFLAGS = -mmic
endif
endif
default: all

all: FORCE
	cd src && $(MAKE) CHPL_DLMALLOC_CFLAGS=$(CHPL_DLMALLOC_CFLAGS)

clean: FORCE
	rm -rf $(DLMALLOC_BUILD_SUBDIR)

cleanall: FORCE
	rm -rf build

clobber: FORCE
	rm -rf build install

FORCE:

.NOTPARALLEL:
