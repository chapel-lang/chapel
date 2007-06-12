
MAKEFLAGS = --no-print-directory

CHPL = chpl

TARGETS = \
	hello \
	beer \
        blockLU \
	genericStack \
	iterators \
	jacobi \
	linkedList \
	norm \
	parallelQuickSort \
	prod-cons \
	tree_construct \


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
