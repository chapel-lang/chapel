CHPL = chpl

CHPL_FLAGS = --fast

TARGETS = \
	binarytrees \
	chameneosredux \
	nbody \
	threadring \

REALS = $(TARGETS:%=%_real)

default: all

all: $(TARGETS)

clean: FORCE
	rm -f $(TARGETS) $(REALS)


binarytrees: binarytrees.chpl
	$(CHPL) -o $@ $(CHPL_FLAGS) $<

chameneosredux: chameneosredux.chpl
	$(CHPL) -o $@ $(CHPL_FLAGS) $<

nbody: nbody.chpl
	$(CHPL) -o $@ $(CHPL_FLAGS) --no-warnings $<

threadring: threadring.chpl
	$(CHPL) -o $@ $(CHPL_FLAGS) $<

FORCE:
