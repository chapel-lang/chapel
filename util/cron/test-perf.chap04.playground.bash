#!/usr/bin/env bash
#

CWD=$(cd $(dirname $0) ; pwd)
source $CWD/common-perf.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="perf.chap04.playground"

# Test performance of jemalloc
#
# Graph the default config and this config side by side to make comparison
# easy, but sync to a different direction so the default chap04 graphs don't
# have multiple configurations.
GITHUB_USER=ronawho
GITHUB_BRANCH=add-jemalloc-source
SHORT_NAME=jemalloc
START_DATE=01/21/16

export CHPL_MEM=jemalloc

git branch -D $GITHUB_USER-$GITHUB_BRANCH
git checkout -b $GITHUB_USER-$GITHUB_BRANCH
git pull https://github.com/$GITHUB_USER/chapel.git $GITHUB_BRANCH

perf_args="-performance-description $SHORT_NAME -performance-configs default:v,$SHORT_NAME:v -sync-dir-suffix $SHORT_NAME"
perf_args="${perf_args} -numtrials 5 -startdate $START_DATE"
$CWD/nightly -cron ${perf_args} ${nightly_args}
