
MAKEFLAGS = --no-print-directory

CHPL = chpl

TARGETS = \
	hello \
	hello-module \
	hello-multicore \
	hello-multiloc \

REALS = $(TARGETS:%=%_real)

default: all primers programs hpcc 

all: $(TARGETS)

clean: FORCE
	rm -f $(TARGETS) $(REALS)
	cd primers && $(MAKE) clean
	cd programs && $(MAKE) clean
	cd hpcc && $(MAKE) clean

primers: FORCE
	cd primers && $(MAKE) all

programs: FORCE
	cd programs && $(MAKE) all

hpcc: FORCE
	cd hpcc && $(MAKE) all

%: %.chpl
	$(CHPL) -o $@ $<

FORCE:
