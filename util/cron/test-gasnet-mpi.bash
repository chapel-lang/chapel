#!/usr/bin/env bash
#
# Test gasnet mpi against hellos on linux64.

CWD=$(cd $(dirname $0) ; pwd)
source $CWD/common.bash

# setup for mpi
export CHPL_COMM=gasnet
export CHPL_COMM_SUBSTRATE=mpi
export GASNET_QUIET=Y
OPEN_MPI_DIR="/usr/lib64/mpi/gcc/openmpi"
export PATH="$OPEN_MPI_DIR/bin:$PATH"
export LD_LIBRARY_PATH="$OPEN_MPI_DIR/lib:$LD_LIBRARY_PATH"

export CHPL_TEST_NUM_LOCALES_AVAILABLE=$SLURM_NNODES

nightly_args="${nightly_args} -no-buildcheck"

export CHPL_NIGHTLY_TEST_CONFIG_NAME="gasnet-mpi"

$CWD/nightly -cron -hellos ${nightly_args}
