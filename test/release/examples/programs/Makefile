
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

default: all

all: $(TARGETS)

clean: FORCE
	rm -f $(TARGETS)

%: %.chpl
	$(CHPL) -o $@ $<

FORCE:
