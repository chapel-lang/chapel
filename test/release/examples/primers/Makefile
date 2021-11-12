
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

CLEAN_TARGETS=$(TARGETS)

#
# Only make FFTW if FFTW_DIR is set or we can find the fftw library
#
ifdef FFTW_DIR
	TARGETS += FFTWlib
	FFTW_OPTS = -I$(FFTW_DIR)/include -L$(FFTW_DIR)/lib
endif
CLEAN_TARGETS += FFTWlib

ifdef CRAY_LIBSCI_PREFIX_DIR
	TARGETS += LAPACKlib LinearAlgebralib
	LAPACK_OPTS = -lgfortran -lsci_gnu
endif
CLEAN_TARGETS += LAPACKlib LinearAlgebralib

#
# Only make interopWithC if LLVM backend is set
#
CHPL_TARGET_COMPILER=$(shell $(CHPL_HOME)/util/chplenv/chpl_compiler.py --target)
ifeq ($(CHPL_TARGET_COMPILER),llvm)
	TARGETS += interopWithC
endif
CLEAN_TARGETS += interopWithC

REALS = $(TARGETS:%=%_real)
CLEAN_REALS = $(CLEAN_TARGETS:%=%_real)

default: all

all: $(TARGETS)

clean: FORCE
	rm -rf $(CLEAN_TARGETS) $(CLEAN_REALS) lib/

%: %.chpl
	+$(CHPL) -o $@ $<

FFTWlib: FFTWlib.chpl
	+$(CHPL) -o $@ $(FFTW_OPTS) $<

LAPACKlib: LAPACKlib.chpl
	+$(CHPL) -o $@ $(LAPACK_OPTS) $<

INTEROP_LIB_OPTS =  --library --library-makefile --savec ccode
INTEROP_DEPS = interopWithC.chpl cHelper.h cHelper.c fact.h fact.c

interopWithC: $(INTEROP_DEPS)
	+$(CHPL) $(INTEROP_DEPS) $(INTEROP_LIB_OPTS)
	+$(CHPL) -o $@ $^

FORCE:
