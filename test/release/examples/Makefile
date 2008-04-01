
MAKEFLAGS = --no-print-directory

CHPL = chpl

TARGETS = \
	hello \
	hello-module \
	arrays \
	beer \
        blockLU \
	fileIO \
	genericStack \
	iterators \
	jacobi \
	linkedList \
	multilocale \
	norm \
	opaque \
	prodCons \
	quicksort \
	reductions \
	slices \
	sparse \
	taskParallel \
	tree \
	varargs \


default: all hpcc

all: $(TARGETS)

clean: FORCE
	rm -f $(TARGETS)
	cd hpcc && $(MAKE) clean

hpcc: FORCE
	cd hpcc && $(MAKE) all

%: %.chpl
	$(CHPL) -o $@ $<

FORCE:
