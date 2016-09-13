#!/usr/bin/env bash
#

CWD=$(cd $(dirname $0) ; pwd)

export CHPL_TEST_PERF_CONFIG_NAME='chapcs'

source $CWD/common-perf.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="perf.chapcs.playground"

NO_SLICE_SHORT=assertNoSlice
OPT_BLK_SHORT=compOptBlk
SYNC_SUFFIX=noInnerMult
START_DATE=09/07/16
NUM_TRIALS=3

# test the performance of -sassertNoSlicing and then my branch that attempts to
# achieve the same array inner multiply removal through compiler optimizations

perf_args="-performance-description $NO_SLICE_SHORT -performance-configs default:v,$NO_SLICE_SHORT:v,$OPT_BLK_SHORT:, -sync-dir-suffix $SYNC_SUFFIX"
perf_args="${perf_args} -performance -numtrials $NUM_TRIALS -startdate $START_DATE"
perf_args="${perf_args} -compopts -sassertNoSlicing"

$CWD/nightly -cron ${nightly_args} ${perf_args}




GITHUB_USER=ronawho
GITHUB_BRANCH=optimize-1dim-array-access-2

git checkout .
git clean -fdx .

git branch -D $GITHUB_USER-$GITHUB_BRANCH
git checkout -b $GITHUB_USER-$GITHUB_BRANCH
git pull https://github.com/$GITHUB_USER/chapel.git $GITHUB_BRANCH

perf_args="-performance-description $OPT_BLK_SHORT -performance-configs default:v,$NO_SLICE_SHORT:v,$OPT_BLK_SHORT:v -sync-dir-suffix $SYNC_SUFFIX"
perf_args="${perf_args} -numtrials $NUM_TRIALS -startdate $START_DATE"
$CWD/nightly -cron ${perf_args} ${nightly_args}
