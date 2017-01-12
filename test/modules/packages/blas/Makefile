#BLASFLAGS=-I/usr/local/include -L/usr/local/lib -lblas
LAPACKFLAGS = $(CFLAGS) $(LDFLAGS) -llapacke -llapack -lblas -lcblas -lgfortran
CHPLFLAGS += --ccflags -Wno-enum-conversion

all: blas1 blas2 blas3

blas1:
	chpl test_blas1.chpl ${BLASFLAGS}

blas2:

blas3:
	chpl test_blas3.chpl ${LAPACKFLAGS} ${CHPLFLAGS}

