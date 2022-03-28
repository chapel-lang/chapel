#!/usr/bin/env bash
#
# Configure environment for MPI module testing on linux64

CWD=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)
source $CWD/common.bash

# Currently, only fifo supported for MPI module
export CHPL_TASKS=fifo
export CHPL_TARGET_COMPILER=mpi-gnu

# setup mpich 3.3.1
source /data/cf/chapel/setup_mpich331.bash
