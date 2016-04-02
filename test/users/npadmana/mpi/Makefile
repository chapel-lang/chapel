# Note that this requires CHPL_MPI to be set. 
# This has been tested with the GNU compiler
CHPL=chpl

TESTS = test_mpi.x

all : ${TESTS}

test_%.x : test_%.chpl MPI.chpl
	${CHPL} -o $@ $<

clean :
	-rm *~ a.out *.x
