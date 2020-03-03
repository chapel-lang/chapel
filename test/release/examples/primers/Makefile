
MAKEFLAGS = --no-print-directory

CHPL = chpl

TARGETS = \
	arrays \
	associative \
	atomics \
	chpldoc.doc \
	classes \
	distributions \
	domains \
	errorHandling \
	fileIO \
	genericClasses \
	iterators \
	learnChapelInYMinutes \
	listOps \
	locales \
	modules \
	nothingVariables \
	parIters \
	procedures \
	randomNumbers \
	ranges \
	records \
	reductions \
	replicated \
	slices \
	sparse \
	specialMethods \
	syncsingle \
	taskParallel \
	timers \
	tuples \
	varargs \
	variables \

#
# Only make FFTW if FFTW_DIR is set or we can find the fftw library
#
ifdef FFTW_DIR
	TARGETS += FFTWlib
	FFTW_OPTS = -I$(FFTW_DIR)/include -L$(FFTW_DIR)/lib
endif

ifdef CRAY_LIBSCI_PREFIX_DIR
	TARGETS += LAPACKlib LinearAlgebralib
	LAPACK_OPTS = -lgfortran -lsci_gnu
endif

REALS = $(TARGETS:%=%_real)

default: all

all: $(TARGETS)

clean: FORCE
	rm -f $(TARGETS) $(REALS)

%: %.chpl
	+$(CHPL) -o $@ $<

FFTWlib: FFTWlib.chpl
	+$(CHPL) -o $@ $(FFTW_OPTS) $<

LAPACKlib: LAPACKlib.chpl
	+$(CHPL) -o $@ $(LAPACK_OPTS) $<

FORCE:
