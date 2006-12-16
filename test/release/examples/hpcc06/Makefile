
CHPL = chpl

CHPL_FLAGS = --ccflags=-O3

TARGETS = \
	stream \
	stream-nopromote \
	ra \
	fft \
	fft-mta

default: all

all: $(TARGETS)

clean: FORCE
	rm -f $(TARGETS)

stream: probSize.chpl stream.chpl
	$(CHPL) -o $@ $(CHPL_FLAGS) $^

stream-nopromote: probSize.chpl stream-nopromote.chpl
	$(CHPL) -o $@ $(CHPL_FLAGS) $^

ra: probSize.chpl ra-randstream.chpl ra.chpl
	$(CHPL) -o $@ $(CHPL_FLAGS) $^

fft: probSize.chpl fft.chpl
	$(CHPL) -o $@ $(CHPL_FLAGS) $^

fft-mta: probSize.chpl fft-mta.chpl
	$(CHPL) -o $@ $(CHPL_FLAGS) $^

FORCE:
