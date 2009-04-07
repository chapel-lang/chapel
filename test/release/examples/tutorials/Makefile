
MAKEFLAGS = --no-print-directory

CHPL = chpl

TARGETS = \
	arrays \
	block2D \
	classes \
	fileIO \
	genericClasses \
	iterators \
	multilocale \
	opaque \
	ranges \
	reductions \
	slices \
	sparse \
	taskParallel \
	timers \
	varargs \
	variables \

default: all

all: $(TARGETS)

clean: FORCE
	rm -f $(TARGETS)

%: %.chpl
	$(CHPL) -o $@ $<

FORCE:
