MPICOMP:=$(shell mpicc --showme:compile)
MPILINK:=$(shell mpicc --showme:link)

EXECS=testSimpleMPI 

all : ${EXECS}


testSimpleMPI : testSimpleMPI.chpl SimpleMPI.chpl
	chpl -o testSimpleMPI testSimpleMPI.chpl mpi.h --ccflags "${MPICOMP}" --ldflags "${MPILINK}"


clean :
	-rm -f ${EXECS} *_real 



