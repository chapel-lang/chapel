#!/usr/bin/env bash
#

CWD=$(cd $(dirname $0) ; pwd)

source $CWD/common-perf.bash
source $CWD/common-fifo.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="perf.chap04.playground"

# test out the performance of Greg's #3178, which simplifies structured
# parallelism (and in particular simplifies fifo a lot.)

beforeSimplifyFifo="f2f814a0e475"
afterSimplifyFifo="274c361da867"

# remove both branches
git branch -D $beforeSimplifyFifo
git branch -D $afterSimplifyFifo

# checkout and test before
git checkout $beforeSimplifyFifo
perf_args="-performance-description fifo -performance-configs default:v,fifo:v,afterFifo:v -sync-dir-suffix fifo"
perf_args="${perf_args} -performance -numtrials 1 -startdate 01/26/16"
$CWD/nightly -cron ${nightly_args} ${perf_args}

# cleanup the repo (jenkins does this the first time)
git checkout .
git clean -fd .

# checkout and test after
git checkout $afterSimplifyFifo
perf_args="-performance-description afterFifo -performance-configs default:v,fifo:v,afterFifo:v -sync-dir-suffix fifo"
perf_args="${perf_args} -performance -numtrials 1 -startdate 01/26/16"
$CWD/nightly -cron ${nightly_args} ${perf_args}
