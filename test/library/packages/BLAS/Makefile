# For using BLAS stand-alone:
#BLASFLAGS= -I/usr/local/include -L/usr/local/lib -lblas

# For using BLAS packaged with LAPACK:
# CFLAGS contains -I/path/to/CBLAS/include -I/path/to/LAPACKE/include
# LDFLAGS contains -L/path/to/lapack-version
LAPACKFLAGS = $(CFLAGS) $(LDFLAGS) -llapacke -llapack -lblas -lcblas -lgfortran
CHPLFLAGS += --ccflags -Wno-enum-conversion

all: blas1 blas2 blas3

blas1:
	chpl test_blas1.chpl ${LAPACKFLAGS} ${CHPLFLAGS} -o test_blas1

blas2:
	chpl test_blas2.chpl ${LAPACKFLAGS} ${CHPLFLAGS} -o test_blas2

blas3:
	chpl test_blas3.chpl ${LAPACKFLAGS} ${CHPLFLAGS} -o test_blas3

