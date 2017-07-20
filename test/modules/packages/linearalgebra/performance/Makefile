# Development Makefile
# - assumes MKL BLAS/LAPACK
# - assumes CFLAGS / LDFLAGS are set
#
LAPACKFLAGS = $(CFLAGS) $(LDFLAGS) -llapack -lblas
CHPLFLAGS += --ccflags -Wno-enum-conversion -sisBLAS_MKL=true --fast

all: transpose-perf

transpose-perf: transpose-perf.chpl
	chpl $< ${LAPACKFLAGS} ${CHPLFLAGS} -o $@
