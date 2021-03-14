
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

default: all benchmarks primers

all: $(TARGETS)

clean: FORCE
	rm -f $(TARGETS) $(REALS)
	cd benchmarks && $(MAKE) clean
	cd patterns && $(MAKE) clean
	cd primers && $(MAKE) clean

benchmarks: FORCE
	cd benchmarks && $(MAKE)

patterns: FORCE
	cd patterns && $(MAKE)

primers: FORCE
	cd primers && $(MAKE)

%: %.chpl
	+$(CHPL) -o $@ $<

FORCE:
