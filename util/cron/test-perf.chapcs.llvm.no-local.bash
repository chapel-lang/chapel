#!/usr/bin/env bash
#

CWD=$(cd $(dirname $0) ; pwd)

export CHPL_TEST_PERF_CONFIG_NAME='chapcs'

source $CWD/common-perf.bash
source $CWD/common-llvm.bash

# common-llvm restricts us to extern/fergeson, we want all the perf tests
unset CHPL_NIGHTLY_TEST_DIRS

export CHPL_NIGHTLY_TEST_CONFIG_NAME="perf.chapcs.llvm.no-local"

perf_args="-performance-description llvm-no-local -performance-configs no-local:v,llvm-no-local:v -sync-dir-suffix llvm-no-local"
perf_args="${perf_args} -performance -numtrials 1 -startdate 12/15/17"

nightly_args="${nightly_args} -no-local"

$CWD/nightly -cron ${nightly_args} ${perf_args}
