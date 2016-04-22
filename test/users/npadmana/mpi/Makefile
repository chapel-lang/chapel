# This has been tested with the GNU compiler
CHPL=chpl
CHPL_FLAGS=--target-compiler=mpi

TESTS = test_mpi.x

all : ${TESTS}

test_%.x : test_%.chpl MPI.chpl
	${CHPL} ${CHPL_FLAGS} -o $@ $<

clean :
	-rm *~ a.out *.x
