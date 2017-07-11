#!/usr/bin/env bash
#

CWD=$(cd $(dirname $0) ; pwd)

export CHPL_TEST_PERF_CONFIG_NAME='chapcs'

source $CWD/common-perf.bash
source $CWD/common-llvm.bash

# common-llvm restricts us to extern/fergeson, we want all the perf tests
unset CHPL_NIGHTLY_TEST_DIRS

export CHPL_NIGHTLY_TEST_CONFIG_NAME="perf.chapcs.playground"

# Test performance of --vectorize with llvm
SHORT_NAME=llvm-vectorize
START_DATE=07/11/17

perf_args="-performance-description $SHORT_NAME -performance-configs llvm:v,$SHORT_NAME:v -sync-dir-suffix $SHORT_NAME"
perf_args="${perf_args} -numtrials 5 -startdate $START_DATE"
perf_args="${perf_args} -compopts --vectorize"
$CWD/nightly -cron ${perf_args} ${nightly_args}
