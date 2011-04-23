
CHPL = chpl

VIVEKA_FLAGS = --ignore-atomic --savec gen_c

CHPL_FLAGS = $(VIVEKA_FLAGS)

TARGETS = \
	stream \
	stream-ep \
	ra \
	fft \
	ptrans \
	hpl \

REALS = $(TARGETS:%=%_real)

default: all variants

all: $(TARGETS)

clean: FORCE
	rm -f $(TARGETS) $(REALS)
	cd variants && $(MAKE) clean

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

variants: FORCE
	cd variants && $(MAKE)

FORCE:
