
CHPL = chpl

CHPL_FLAGS = --fast

TARGETS = \
	stream \
	ra \
	fft \
	hpl \

REALS = $(TARGETS:%=%_real)

default: all

all: $(TARGETS)

clean: FORCE
	rm -f $(TARGETS) $(REALS)

stream: HPCCProblemSize.chpl stream.chpl
	$(CHPL) -o $@ $(CHPL_FLAGS) $<

ra: HPCCProblemSize.chpl RARandomStream.chpl ra.chpl
	$(CHPL) -o $@ $(CHPL_FLAGS) $<

fft: HPCCProblemSize.chpl fft.chpl
	$(CHPL) -o $@ $(CHPL_FLAGS) $<

hpl: HPCCProblemSize.chpl hpl.chpl
	$(CHPL) -o $@ $(CHPL_FLAGS) $<

FORCE:
