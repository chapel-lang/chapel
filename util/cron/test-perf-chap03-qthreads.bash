#!/usr/bin/env bash
#

CWD=$(cd $(dirname $0) ; pwd)
source $CWD/common-perf.bash
source $CWD/common-qthreads.bash
$CWD/nightly -cron -performance-description 'qthreads --genGraphOpts "-m default -m qthreads"' -numtrials 5 -startdate 08/21/07
