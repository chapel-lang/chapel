
CHPL = chpl

CHPL_FLAGS = --fast

TARGETS = \
	isx

REALS = $(TARGETS:%=%_real)

default: all

all: $(TARGETS)

clean: FORCE
	rm -f $(TARGETS) $(REALS)

isx: isx.chpl
	+$(CHPL) -o $@ $(CHPL_FLAGS) $<

FORCE:
