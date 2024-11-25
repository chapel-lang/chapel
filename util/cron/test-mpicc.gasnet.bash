#!/usr/bin/env bash
#
# Test MPI Module for CHPL_COMM=gasnet on linux64

UTIL_CRON_DIR=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)
source $UTIL_CRON_DIR/common-gasnet.bash
source $UTIL_CRON_DIR/common-mpicc.bash

export CHPL_COMM_SUBSTRATE=mpi

export CHPL_NIGHTLY_TEST_DIRS="release/examples/hello*.chpl library/packages/MPI users/npadmana/mpi"

export CHPL_NIGHTLY_TEST_CONFIG_NAME="mpicc.gasnet"

export GASNET_QUIET=Y

$UTIL_CRON_DIR/nightly -cron -no-buildcheck
