THIRD_PARTY_ABS_DIR = $(shell pwd)

default: all

all: gasnet

clean: FORCE
	cd gasnet && make clean

gasnet: FORCE
	-cd gasnet && echo "n" | ./Bootstrap
	cd gasnet && ./configure --prefix=$(THIRD_PARTY_ABS_DIR)/gasnet/install --enable-segment-everything --enable-debug
	cd gasnet && $(MAKE) all
	cd gasnet && $(MAKE) install

FORCE:
