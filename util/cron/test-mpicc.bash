#!/usr/bin/env bash
#
# Test MPI Module for CHPL_COMM=none on linux64

UTIL_CRON_DIR=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)
source $UTIL_CRON_DIR/common-mpicc.bash

export CHPL_LAUNCHER=mpirun

export CHPL_NIGHTLY_TEST_DIRS="release/examples/hello*.chpl library/packages/MPI"

export CHPL_NIGHTLY_TEST_CONFIG_NAME="mpicc"

$UTIL_CRON_DIR/nightly -cron -no-buildcheck
