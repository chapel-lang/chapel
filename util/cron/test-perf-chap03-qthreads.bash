#!/usr/bin/env bash
#
# Run performance tests on chap03 with tasks=qthreads.

CWD=$(cd $(dirname $0) ; pwd)
source $CWD/common-perf.bash
source $CWD/common-qthreads.bash
export CHPL_HWLOC=none
$CWD/nightly -cron -performance-description 'qthreads --genGraphOpts "-m default -m qthreads"' -numtrials 5 -startdate 08/21/07
