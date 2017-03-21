LAPACKFLAGS = $(CFLAGS) $(LDFLAGS) -llapacke -llapack -lblas -lcblas -lgfortran
CHPLFLAGS += --ccflags -Wno-enum-conversion

all: correctness

correctness: correctness.chpl
	chpl correctness.chpl ${LAPACKFLAGS} ${CHPLFLAGS} -o correctness

