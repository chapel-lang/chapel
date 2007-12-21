
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
	norm \
	opaque \
	prodCons \
	quicksort \
	reductions \
	slices \
	sparse \
	tree \


default: all hpcc

all: $(TARGETS)

clean: FORCE
	rm -f $(TARGETS)
	cd hpcc && $(MAKE) clean
	./start_clean

hpcc: FORCE
	cd hpcc && $(MAKE) all

%: %.chpl
	$(CHPL) -o $@ $<

FORCE:
