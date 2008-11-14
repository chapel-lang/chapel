
CHPL = chpl

CHPL_FLAGS = --fast

TARGETS = \
	stream \
	ra \
	fft \
	hpl \

default: all

all: $(TARGETS)

clean: FORCE
	rm -f $(TARGETS)

stream: probSize.chpl stream.chpl
	$(CHPL) -o $@ $(CHPL_FLAGS) $^

ra: probSize.chpl ra-randstream.chpl ra.chpl
	$(CHPL) -o $@ $(CHPL_FLAGS) $^

fft: probSize.chpl fft.chpl
	$(CHPL) -o $@ $(CHPL_FLAGS) $^

hpl: probSize.chpl hpl.chpl
	$(CHPL) -o $@ $(CHPL_FLAGS) $^

FORCE:
