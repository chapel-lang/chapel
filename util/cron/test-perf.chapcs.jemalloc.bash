#!/usr/bin/env bash
#
# Run performance tests on a chapcs machine with jemalloc

CWD=$(cd $(dirname $0) ; pwd)

export CHPL_TEST_PERF_CONFIG_NAME='chapcs'

source $CWD/common-perf.bash
source $CWD/common-jemalloc.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="perf.chapcs.jemalloc"

perf_args="-performance-description jemalloc -performance-configs default:v,jemalloc:v,tcmalloc -sync-dir-suffix mem"
perf_args="${perf_args} -performance -numtrials 5 -startdate 01/22/16"

$CWD/nightly -cron ${nightly_args} ${perf_args}
