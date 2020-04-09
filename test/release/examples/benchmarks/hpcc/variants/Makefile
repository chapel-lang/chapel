
CHPL = chpl

CHPL_FLAGS = --fast -M ..

TARGETS = \
	stream-promoted \
	ra-unordered-atomics \
	ra-cleanloop \

REALS = $(TARGETS:%=%_real)

default: all

all: $(TARGETS)

clean: FORCE
	rm -f $(TARGETS) $(REALS)

stream-promoted: stream-promoted.chpl ../HPCCProblemSize.chpl
	+$(CHPL) -o $@ $(CHPL_FLAGS) $<

ra-unordered-atomics: ra-unordered-atomics.chpl ../HPCCProblemSize.chpl ../RARandomStream.chpl
	+$(CHPL) -o $@ $(CHPL_FLAGS) $<

ra-cleanloop: ra-cleanloop.chpl ../HPCCProblemSize.chpl ../RARandomStream.chpl
	+$(CHPL) -o $@ $(CHPL_FLAGS) $<

FORCE:
