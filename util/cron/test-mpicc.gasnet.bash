#!/usr/bin/env bash
#
# Test CHPL_COMM=gasnet for MPI module on linux64

CWD=$(cd $(dirname $0) ; pwd)
source $CWD/common-mpicc.bash
source $CWD/common-gasnet.bash

export CHPL_COMM_SUBSTRATE=mpi

export CHPL_NIGHTLY_TEST_DIRS="release/examples/hello*.chpl modules/packages/mpi"

export CHPL_NIGHTLY_TEST_CONFIG_NAME="mpicc.gasnet"

export GASNET_QUIET=Y

$CWD/nightly -cron -no-buildcheck
