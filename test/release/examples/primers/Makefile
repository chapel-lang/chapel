
MAKEFLAGS = --no-print-directory

CHPL = chpl

TARGETS = \
	arrays \
        arrayVectorOps \
	associative \
	atomics \
	chpldoc.doc \
	classes \
	distributions \
	domains \
	fileIO \
	genericClasses \
	iterators \
	leaderfollower \
	locales \
	opaque \
	procedures \
        randomNumbers \
	ranges \
	reductions \
	slices \
	sparse \
	syncsingle \
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
