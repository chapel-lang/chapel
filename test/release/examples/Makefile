
MAKEFLAGS = --no-print-directory

CHPL = chpl

TARGETS = \
	hello \
	hello2-module \
	hello3-datapar \
	hello4-datapar-dist \
	hello5-taskpar \
	hello6-taskpar-dist \

REALS = $(TARGETS:%=%_real)

default: all primers programs hpcc 

all: $(TARGETS)

clean: FORCE
	rm -f $(TARGETS) $(REALS)
	cd primers && $(MAKE) clean
	cd programs && $(MAKE) clean
	cd hpcc && $(MAKE) clean

primers: FORCE
	cd primers && $(MAKE)

programs: FORCE
	cd programs && $(MAKE)

hpcc: FORCE
	cd hpcc && $(MAKE)

%: %.chpl
	$(CHPL) -o $@ $<

FORCE:
