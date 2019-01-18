#!/usr/bin/env bash
#

CWD=$(cd $(dirname $0) ; pwd)

export CHPL_TEST_PERF_CONFIG_NAME='chapcs'

source $CWD/common-perf.bash
source $CWD/common-numa.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="perf.chapcs.numa"

perf_args="-performance-description numa -performance-configs default:v,numa:v -sync-dir-suffix numa"
perf_args="${perf_args} -performance -numtrials 1 -startdate 01/15/16"

$CWD/nightly -cron ${nightly_args} ${perf_args}
