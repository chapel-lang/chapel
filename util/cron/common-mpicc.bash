#!/usr/bin/env bash
#
# Configure environment for MPI module testing on linux64

CWD=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)
source $CWD/common.bash

# Currently, only fifo supported for MPI module
export CHPL_TASKS=fifo
export CHPL_TARGET_COMPILER=mpi-gnu

# Load OpenMPI environment module and confirm it has loaded
echo >&2 module load gnu-openmpi
module load gnu-openmpi

set -x
: confirm mpi module is loaded
module list -l 2>&1 | grep -E -q '\bgnu-openmpi\b' || exit $?

if [[ ${CHPL_RT_OVERSUBSCRIBED:-n} == [1tTyY]* && -z $MPIRUN_CMD ]] ; then
  export MPIRUN_CMD='mpirun -np %N -map-by node:oversubscribe %C'
fi
