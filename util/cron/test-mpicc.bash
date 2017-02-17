#!/usr/bin/env bash
#
# Test CHPL_COMM=none && CHPL_TARGET_COMPILER=mpi-gnu for MPI module testing
#
CWD=$(cd $(dirname $0) ; pwd)
source $CWD/common.bash

echo >&2 module load mpi
module load mpi

set -x
: confirm mpi module is loaded
module list -l 2>&1 | grep -E '\bmpi/mpich\b' || exit $?

export MPICH_MAX_THREAD_SAFETY=multiple

export CHPL_TARGET_COMPILER=mpi-gnu
export CHPL_TASKS=fifo
export CHPL_COMM=none
export CHPL_LAUNCHER=mpirun

export CHPL_NIGHTLY_TEST_DIRS="release/examples/hello*.chpl modules/packages/mpi/spmd/hello-chapel"

export CHPL_NIGHTLY_TEST_CONFIG_NAME="mpicc"

$CWD/nightly -cron -no-buildcheck ${nightly_args}
