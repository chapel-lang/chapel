
MAKEFLAGS = --no-print-directory

CHPL = chpl

TARGETS = \
	hello \
	hello-config \
	hello-module \
	beer \
        blockLU \
	genericStack \
	iterators \
	jacobi \
	linkedList \
	norm \
	prodCons \
	quicksort \
	sparse \
	tree \


default: all hpcc06

all: $(TARGETS)

clean: FORCE
	rm -f $(TARGETS)
	cd hpcc06 && $(MAKE) clean
	./start_clean

hpcc06: FORCE
	cd hpcc06 && $(MAKE) all

%: %.chpl
	$(CHPL) -o $@ $<

FORCE:
