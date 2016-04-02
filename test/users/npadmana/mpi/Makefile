# Note that this requires CHPL_MPI to be set. 
# This has been tested with the GNU compiler
CHPL=chpl

TESTS = test_mpi_initialize.x

all : ${TESTS}

test_%.x : test_%.chpl MPI.chpl
	${CHPL} -o $@ $<


testFlags :
	@echo ${MPICOMPILE}
	@echo ${MPILINK}
	@echo ${CHPL}


clean :
	-rm *~ a.out *.x
