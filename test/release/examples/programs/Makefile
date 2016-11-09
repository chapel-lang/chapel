
MAKEFLAGS = --no-print-directory

CHPL = chpl

TARGETS = \
	beer \
	genericStack \
	jacobi \
	linkedList \
	norm \
	prodCons \
	quicksort \
	tree \

REALS = $(TARGETS:%=%_real)

default: all

all: $(TARGETS)

clean: FORCE
	rm -f $(TARGETS) $(REALS)

%: %.chpl
	+$(CHPL) -o $@ $<

FORCE:
