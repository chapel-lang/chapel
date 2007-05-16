
MAKEFLAGS = --no-print-directory

CHPL = chpl

TARGETS = \
	hello \
	beer \
	jacobi \
	genericStack \
	genericStack2 \
	norm \
	parallelQuickSort \
	prod-cons \
	tree_construct \
	iterators

default: all hpcc06

all: $(TARGETS)

clean: FORCE
	rm -f $(TARGETS)
	cd hpcc06 && $(MAKE) clean

hpcc06: FORCE
	cd hpcc06 && $(MAKE) all

%: %.chpl
	$(CHPL) -o $@ $<

FORCE:
