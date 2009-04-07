
MAKEFLAGS = --no-print-directory

CHPL = chpl

TARGETS = \
	arrays \
	fileIO \
	iterators \
	multilocale \
	opaque \
	ranges \
	reductions \
	slices \
	sparse \
	taskParallel \
	varargs \
	variables \

default: all

all: $(TARGETS)

clean: FORCE
	rm -f $(TARGETS)

%: %.chpl
	$(CHPL) -o $@ $<

FORCE:
