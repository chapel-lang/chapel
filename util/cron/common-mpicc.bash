#!/usr/bin/env bash
#
# Configure environment for MPI module testing on linux64


echo >&2 module load mpi
module load mpi

set -x
: confirm mpi module is loaded
module list -l 2>&1 | grep -E '\bmpi/mpich\b' || exit $?

export CHPL_TARGET_COMPILER=mpi-gnu
# MPI module only works with CHPL_TASKS=fifo currently
export CHPL_TASKS=fifo

