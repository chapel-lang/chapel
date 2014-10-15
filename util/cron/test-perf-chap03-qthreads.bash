#!/usr/bin/env bash
#

CWD=$(cd $(dirname $0) ; pwd)
source $CWD/common-perf.bash
source $CWD/common-qthreads.bash
$CWD/nightly -cron -sync-dest-path playground -performance-description 'playground --genGraphOpts "-m default -m playground"' -numtrials 5 -startdate 08/21/07
