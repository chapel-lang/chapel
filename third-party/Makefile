CHAPEL_ROOT = ..
CHAPEL_SUBDIR = third-party
include $(CHAPEL_ROOT)/make/Makefile.base

THIRD_PARTY_ABS_DIR = $(shell pwd)

default: all

all: gasnet

clean: FORCE
	cd gasnet && make clean

armci: FORCE
	cd armci && make

gasnet: FORCE
	-cd gasnet && echo "n" | ./Bootstrap
	cd gasnet && ./configure --prefix=$(THIRD_PARTY_ABS_DIR)/gasnet/install/$(CHPL_MAKE_PLATFORM) --enable-segment-everything --enable-debug
	cd gasnet && $(MAKE) all
	cd gasnet && $(MAKE) install

FORCE:
