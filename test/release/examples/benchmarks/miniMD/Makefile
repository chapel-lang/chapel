
MAKEFLAGS = --no-print-directory

CHPL = chpl

default: simple explicit stencil

all: $(TARGETS)

clean: FORCE
	cd simple && $(MAKE) clean
	cd explicit && $(MAKE) clean
	cd stencil && $(MAKE) clean

simple: FORCE
	cd simple && $(MAKE)

explicit: FORCE
	cd explicit && $(MAKE)

stencil: FORCE
	cd stencil && $(MAKE)

FORCE:
