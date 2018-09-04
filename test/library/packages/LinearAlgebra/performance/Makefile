# Development Makefile
# - assumes openblas / lapack
# - assumes CFLAGS / LDFLAGS are set
#
LAPACKFLAGS = $(CFLAGS) $(LDFLAGS) -llapack -lopenblas
CHPLFLAGS += --ccflags -Wno-enum-conversion --fast

all: transpose-perf

transpose-perf: transpose-perf.chpl
	chpl $< ${LAPACKFLAGS} ${CHPLFLAGS} -o $@
