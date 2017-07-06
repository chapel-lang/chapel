#!/usr/bin/env bash
#
# Configure environment for MPI module testing on linux64

CWD=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)
source $CWD/common.bash

# Currently, only fifo supported for MPI module
export CHPL_TASKS=fifo
export CHPL_TARGET_COMPILER=mpi-gnu

# Load MPI environment module and confirm it has loaded
echo >&2 module load mpi
module load mpi

set -x
: confirm mpi module is loaded
module list -l 2>&1 | grep -E '\bmpi/mpich\b' || exit $?
