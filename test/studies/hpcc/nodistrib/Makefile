
CHPL = chpl

CHPL_FLAGS = --fast

TARGETS = \
	stream.nodist \
	ra.nodist \
	fft.nodist \
	hpl.nodist \

REALS = $(TARGETS:%=%_real)

default: all

all: $(TARGETS)

clean: FORCE
	rm -f $(TARGETS) $(REALS)

stream: probSize.chpl stream.nodist.chpl
	$(CHPL) -o $@ $(CHPL_FLAGS) $^

ra: probSize.chpl ra-randstream.chpl ra.nodist.chpl
	$(CHPL) -o $@ $(CHPL_FLAGS) $^

fft: probSize.chpl fft.nodist.chpl
	$(CHPL) -o $@ $(CHPL_FLAGS) $^

hpl: probSize.chpl hpl.nodist.chpl
	$(CHPL) -o $@ $(CHPL_FLAGS) $^

FORCE:
