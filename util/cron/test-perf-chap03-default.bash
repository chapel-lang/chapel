#!/usr/bin/env bash
#
# Run performance tests on chap03 with default configuration.

CWD=$(cd $(dirname $0) ; pwd)
source $CWD/common-perf.bash
$CWD/nightly -cron -performance-description 'default --genGraphOpts "-m default -m qthreads"' -numtrials 5 -startdate 08/21/07
