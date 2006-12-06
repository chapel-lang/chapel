
MAKEFLAGS = --no-print-directory

CHPL = chpl

TARGETS = \
	hello \
	beer \
	jacobi \
	jacobi-routine \
	genericStack \
	genericStack2 \
	norm \
	prod-cons \
	tree_construct

default: all hpcc06

all: $(TARGETS)

clean: FORCE
	rm $(TARGETS)

hpcc06: FORCE
	cd $@ && $(MAKE) all

%: %.chpl
	$(CHPL) -o $@ $<

FORCE:
