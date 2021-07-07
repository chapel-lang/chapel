
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
	forallLoops \
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

#
# Only make interopWithC and cClient if LLVM backend is set
#
CHPL_TARGET_COMPILER=$(shell $(CHPL_HOME)/util/chplenv/chpl_compiler.py --target)
ifeq ($(CHPL_TARGET_COMPILER),llvm)
	TARGETS += cClient
	TARGETS += interopWithC
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

CCLIENT_OPTS = -Llib/ -linteropWithC `$CHPL_HOME/util/config/compileline --libraries`
INTEROP_LIB_OPTS =  --library --library-makefile --savec ccode
INTEROP_DEPS = interopWithC.chpl cHelper.h cHelper.c fact.h fact.c

interopWithC: $(INTEROP_DEPS)
	+$(CHPL) -o $@ $^


lib/Makefile.interopWithC:
	+$(CHPL) $(INTEROP_DEPS) $(INTEROP_LIB_OPTS)

-include lib/Makefile.interopWithC

cClient: cClient.test.c interopWithC.chpl
	+$(CHPL_COMPILER) $(CHPL_CFLAGS) -o $@ $< $(CHPL_LDFLAGS)

FORCE:
