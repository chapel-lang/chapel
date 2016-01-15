#!/usr/bin/env bash
#

CWD=$(cd $(dirname $0) ; pwd)
source $CWD/common-perf.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="perf.chap04.playground"

# Test performance of Michael's ref-rvalue change
#
# Graph the default config and this config side by side to make comparison
# easy, but sync to a different direction so the default chap04 graphs don't
# have multiple configurations.
GITHUB_USER=mppf
GITHUB_BRANCH=ref-return-not-value
SHORT_NAME=ref-rvalue
START_DATE=12/22/15

git branch -D $GITHUB_USER-$GITHUB_BRANCH
git checkout -b $GITHUB_USER-$GITHUB_BRANCH master
git pull https://github.com/$GITHUB_USER/chapel.git $GITHUB_BRANCH

perf_args="-performance-description $SHORT_NAME -performance-configs default:v,$SHORT_NAME:v -sync-dir-suffix $SHORT_NAME"
perf_args="${perf_args} -numtrials 5 -startdate $START_DATE"
$CWD/nightly -cron ${perf_args} ${nightly_args}
