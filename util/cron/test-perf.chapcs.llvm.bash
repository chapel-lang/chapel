#!/usr/bin/env bash
#

CWD=$(cd $(dirname $0) ; pwd)

export CHPL_TEST_PERF_CONFIG_NAME='chapcs'

source $CWD/common-perf.bash
source $CWD/common-llvm.bash

# common-llvm restricts us to extern/fergeson, we want all the perf tests
unset CHPL_NIGHTLY_TEST_DIRS

export CHPL_NIGHTLY_TEST_CONFIG_NAME="perf.chapcs.llvm"

perf_args="-performance-description llvm -performance-configs default:v,llvm:v -sync-dir-suffix llvm"
perf_args="${perf_args} -performance -numtrials 1 -startdate 11/24/15"

$CWD/nightly -cron ${nightly_args} ${perf_args}
