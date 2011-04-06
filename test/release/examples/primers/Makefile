
MAKEFLAGS = --no-print-directory

CHPL = chpl

TARGETS = \
	arrays \
	classes \
	distributions \
	fileIO \
	genericClasses \
	iterators \
	locales \
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
