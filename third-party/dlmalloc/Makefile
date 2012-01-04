CHAPEL_ROOT = ../..
CHPL_MAKE_HOST_TARGET = --target
include $(CHAPEL_ROOT)/make/Makefile.base

default: all

all: FORCE
	cd src && $(MAKE)

clean: FORCE
	rm -rf $(DLMALLOC_BUILD_DIR)

clobber: FORCE
	rm -rf build install

FORCE:
