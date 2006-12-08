
CHPL = chpl

TARGETS = stream ra fft fft-mta

default: all

all: $(TARGETS)

clean: FORCE
	rm -f $(TARGETS)

stream: probSize.chpl stream.chpl
	$(CHPL) -o $@ $^

ra: probSize.chpl ra-randstream.chpl ra.chpl
	$(CHPL) -o $@ $^

fft: probSize.chpl fft.chpl
	$(CHPL) -o $@ $^

fft-mta: probSize.chpl fft-mta.chpl
	$(CHPL) -o $@ $^

FORCE:
