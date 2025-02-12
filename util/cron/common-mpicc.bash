#!/usr/bin/env bash
#
# Configure environment for MPI module testing on linux64

UTIL_CRON_DIR=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)
source $UTIL_CRON_DIR/common.bash

# Currently, only fifo supported for MPI module
export CHPL_TASKS=fifo
export CHPL_TARGET_COMPILER=mpi-gnu

# setup mpich 
source /hpcdc/project/chapel/setup_mpi.bash
