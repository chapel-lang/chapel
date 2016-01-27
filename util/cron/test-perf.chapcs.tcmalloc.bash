#!/usr/bin/env bash
#
# Run performance tests on a chapcs machine with tcmalloc

CWD=$(cd $(dirname $0) ; pwd)

export CHPL_TEST_PERF_CONFIG_NAME='chapcs'

source $CWD/common-perf.bash
source $CWD/common-tcmalloc.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="perf.chapcs.tcmalloc"

perf_args="-performance-description tcmalloc -performance-configs default:v,jemalloc:v,tcmalloc:v -sync-dir-suffix mem"
perf_args="${perf_args} -performance -numtrials 5 -startdate 01/22/16"

$CWD/nightly -cron ${nightly_args} ${perf_args}
