
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

REALS = $(TARGETS:%=%_real)

default: all

all: $(TARGETS)

clean: FORCE
	rm -f $(TARGETS) $(REALS)

%: %.chpl
	$(CHPL) -o $@ $<

FORCE:
