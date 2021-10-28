#!/usr/bin/env bash
#

CWD=$(cd $(dirname $0) ; pwd)

export CHPL_TEST_PERF_CONFIG_NAME='chapcs'

source $CWD/common-perf.bash
source $CWD/common-c-backend.bash

# common-llvm restricts us to extern/fergeson, we want all the perf tests
unset CHPL_NIGHTLY_TEST_DIRS

export CHPL_NIGHTLY_TEST_CONFIG_NAME="perf.chapcs.c-backend"

perf_args="-performance-description c-backend -performance-configs default:v -sync-dir-suffix c-backend"
perf_args="${perf_args} -performance -numtrials 1 -startdate 11/24/15"

$CWD/nightly -cron ${nightly_args} ${perf_args}
