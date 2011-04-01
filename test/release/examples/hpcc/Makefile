
CHPL = chpl

CHPL_FLAGS = --fast

TARGETS = \
	stream \
	stream-ep \
	ra \
	fft \
	hpl \

REALS = $(TARGETS:%=%_real)

default: all

all: $(TARGETS)

clean: FORCE
	rm -f $(TARGETS) $(REALS)

stream: stream.chpl HPCCProblemSize.chpl
	$(CHPL) -o $@ $(CHPL_FLAGS) $<

stream-ep: stream-ep.chpl HPCCProblemSize.chpl
	$(CHPL) -o $@ $(CHPL_FLAGS) $<

ra: ra.chpl HPCCProblemSize.chpl RARandomStream.chpl
	$(CHPL) -o $@ $(CHPL_FLAGS) $<

fft: fft.chpl HPCCProblemSize.chpl
	$(CHPL) -o $@ $(CHPL_FLAGS) $<

ptrans: ptrans.chpl HPCCProblemSize.chpl
	$(CHPL) -o $@ $(CHPL_FLAGS) $<

hpl: hpl.chpl HPCCProblemSize.chpl
	$(CHPL) -o $@ $(CHPL_FLAGS) $<

FORCE:
