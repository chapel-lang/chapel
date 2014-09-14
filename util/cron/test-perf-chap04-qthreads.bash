#!/usr/bin/env bash
#

CWD=$(cd $(dirname $0) ; pwd)
source $CWD/common-perf.bash
source $CWD/common-qthreads.bash
export CHPL_RT_NUM_THREADS_PER_LOCALE=16
$CWD/nightly -cron -sync-dest-path playground -performance-description 'playground --genGraphOpts "-m default -m playground"' -numtrials 5 -startdate 07/28/12
