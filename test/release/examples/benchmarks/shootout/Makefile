CHPL = chpl

CHPL_FLAGS = --fast

TARGETS = \
	nbody

REALS = $(TARGETS:%=%_real)

default: all

all: $(TARGETS)

clean: FORCE
	rm -f $(TARGETS) $(REALS)

nbody: nbody.chpl
	$(CHPL) -o $@ $(CHPL_FLAGS) --no-warnings $<

FORCE:
