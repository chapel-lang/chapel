#!/bin/bash
export AMMPI_MPI_THREAD=multiple
# This is for Cray systems, at least the ones I use!
export MPICH_MAX_THREAD_SAFETY=multiple

export CHPL_TARGET_COMPILER=mpi-gnu

# Chapel (single-locale) + MPI
export CHPL_COMM=none
rm -f a.out
chpl -o a.out test.chpl
mpirun -np 1 ./a.out
mpirun -np 4 ./a.out

# Chapel + MPI
export CHPL_COMM=gasnet
export CHPL_COMM_SUBSTRATE=mpi
export CHPL_TASKS=fifo
rm -f a.out a.out_real
chpl -o a.out test.chpl
./a.out -nl 1
./a.out -nl 4

