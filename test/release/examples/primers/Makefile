
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
	locales \
	opaque \
	parIters \
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

#
# Only make FFTW if FFTW_DIR is set or we can find the fftw library
#
ifdef FFTW_DIR
	TARGETS += FFTWlib
	FFTW_OPTS = -I$(FFTW_DIR)/include -L$(FFTW_DIR)/lib
else ifneq ($(shell /sbin/ldconfig -p | grep -i fftw),)
	TARGETS += FFTWlib
endif

REALS = $(TARGETS:%=%_real)

default: all

all: $(TARGETS)

clean: FORCE
	rm -f $(TARGETS) $(REALS)

%: %.chpl
	$(CHPL) -o $@ $<

FFTWlib: FFTWlib.chpl
	$(CHPL) -o $@ $(FFTW_OPTS) $<

FORCE:
