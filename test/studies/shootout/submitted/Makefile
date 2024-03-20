SRCS := $(wildcard *.chpl)
BINS = $(SRCS:%.chpl=%)

all: $(BINS)
	echo $(SRCS)

%: %.chpl
	chpl $<

.PHONY: Compat
