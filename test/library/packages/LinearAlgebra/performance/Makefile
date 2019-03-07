# Development Makefile
# - assumes openblas / lapack
# - assumes CFLAGS / LDFLAGS are set
#
LAPACKFLAGS = $(CFLAGS) $(LDFLAGS) -llapack -lblas
CHPLFLAGS += --ccflags -Wno-enum-conversion

%: %.chpl
	chpl $< ${LAPACKFLAGS} ${CHPLFLAGS}
