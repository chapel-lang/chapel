SRCS := $(wildcard *.chpl)
BINS = $(SRCS:%.chpl=%)

CHPL_FLAGS=--warn-unstable

all: $(BINS)

clean:
	rm -f $(BINS)

%: %.chpl
	chpl $(CHPL_FLAGS) $<
