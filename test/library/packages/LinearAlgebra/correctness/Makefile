# Development Makefile
# - assumes openblas / lapack
# - assumes CFLAGS / LDFLAGS are set
#
LAPACKFLAGS = $(CFLAGS) $(LDFLAGS) -llapack -lblas
CHPLFLAGS += --ccflags -Wno-enum-conversion

all: correctness test_identities1 testEigen testCholesky

correctness: correctness.chpl
	chpl $< ${LAPACKFLAGS} ${CHPLFLAGS} -o $@

test_identities1: test_identities1.chpl
	chpl $< ${LAPACKFLAGS} ${CHPLFLAGS} -o $@

testEigen: testEigen.chpl
	chpl $< ${LAPACKFLAGS} ${CHPLFLAGS} -o $@

testCholesky: testCholesky.chpl
	chpl $< ${LAPACKFLAGS} ${CHPLFLAGS} -o $@
