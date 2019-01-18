#!/usr/bin/env bash
#
# Test MPI Module for CHPL_COMM=none on linux64

CWD=$(cd $(dirname $0) ; pwd)
source $CWD/common-mpicc.bash

export CHPL_LAUNCHER=mpirun

export CHPL_NIGHTLY_TEST_DIRS="release/examples/hello*.chpl library/packages/MPI"

export CHPL_NIGHTLY_TEST_CONFIG_NAME="mpicc"

$CWD/nightly -cron -no-buildcheck
