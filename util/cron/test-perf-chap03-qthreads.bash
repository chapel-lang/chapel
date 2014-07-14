#!/usr/bin/env bash
#
# Run performance tests on chap03 with tasks=qthreads.

CWD=$(cd $(dirname $0) ; pwd)
source $CWD/common-perf.bash
source $CWD/common-qthreads.bash
export CHPL_QTHREAD_NO_GUARD_PAGES=yes
export CHPL_QTHREAD_SCHEDULER=nemesis
export QTHREAD_NUM_SHEPHERDS=2
export QTHREAD_NUM_WORKERS_PER_SHEPHERD=1
$CWD/nightly -cron -performance-description 'qthreads --genGraphOpts "-m default -m qthreads"' -numtrials 5 -startdate 08/21/07
