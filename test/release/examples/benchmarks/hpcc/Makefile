
CHPL = chpl

CHPL_FLAGS = --fast

TARGETS = \
	stream \
	stream-ep \
	ra \
	ra-atomics \

REALS = $(TARGETS:%=%_real)

default: all variants

all: $(TARGETS)

clean: FORCE
	rm -f $(TARGETS) $(REALS)
	cd variants && $(MAKE) clean

stream: stream.chpl HPCCProblemSize.chpl
	+$(CHPL) -o $@ $(CHPL_FLAGS) $<

stream-ep: stream-ep.chpl HPCCProblemSize.chpl
	+$(CHPL) -o $@ $(CHPL_FLAGS) $<

ra: ra.chpl HPCCProblemSize.chpl RARandomStream.chpl
	+$(CHPL) -o $@ $(CHPL_FLAGS) $<

ra-atomics: ra-atomics.chpl HPCCProblemSize.chpl RARandomStream.chpl
	+$(CHPL) -o $@ $(CHPL_FLAGS) $<

variants: FORCE
	cd variants && $(MAKE)

FORCE:
