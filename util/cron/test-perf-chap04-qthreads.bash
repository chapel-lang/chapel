#!/usr/bin/env bash
#

CWD=$(cd $(dirname $0) ; pwd)
source $CWD/common-perf.bash
source $CWD/common-qthreads.bash
export CHPL_RT_NUM_THREADS_PER_LOCALE=16
# releasePerformance still generates results based on the fifo timings. It's
# run here again, otherwise syncing the qthreads results blows away the
# directory with the releaseOverRelease graphs in
$CWD/nightly -cron -performance-description 'qthreads --genGraphOpts "-m default -m qthreads"' -releasePerformance -numtrials 5 -startdate 07/28/12
