CHPL = chpl

CHPL_FLAGS = --fast

TARGETS = \
  chameneosredux
	nbody
  threadring

REALS = $(TARGETS:%=%_real)

default: all

all: $(TARGETS)

clean: FORCE
	rm -f $(TARGETS) $(REALS)


chameneosredux: chameneosredux.chpl
  $(CHPL) -o $@ $(CHPL_FLAGS) $<

nbody: nbody.chpl
	$(CHPL) -o $@ $(CHPL_FLAGS) $<

threadring: threadring.chpl
  $(CHPL) -o $@ $(CHPL_FLAGS) $<

FORCE:
