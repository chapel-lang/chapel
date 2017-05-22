#!/usr/bin/env bash
#
# Test CHPL_COMM=none for MPI module on linux64

CWD=$(cd $(dirname $0) ; pwd)
source $CWD/common-mpicc.bash
source $CWD/common.bash

export CHPL_LAUNCHER=mpirun

export CHPL_NIGHTLY_TEST_DIRS="release/examples/hello*.chpl modules/packages/mpi"

export CHPL_NIGHTLY_TEST_CONFIG_NAME="mpicc"

$CWD/nightly -cron -no-buildcheck
