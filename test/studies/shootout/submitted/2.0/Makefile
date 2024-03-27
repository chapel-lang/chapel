SRCS := $(wildcard *.chpl)
BINS = $(SRCS:%.chpl=%)

all: $(BINS)

CHPL_FLAGS=--warn-unstable

%: %.chpl
	chpl $(CHPL_FLAGS) $<
