#!/usr/bin/env bash
#

CWD=$(cd $(dirname $0) ; pwd)

export CHPL_TEST_PERF_CONFIG_NAME='chapcs'

source $CWD/common-perf.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="perf.chapcs.playground"

# Test performance of removing reference handling from copy propagation
GITHUB_USER=benharsh
GITHUB_BRANCH=copy-prop-no-refs
SHORT_NAME=copy-prop-no-refs
START_DATE=07/27/17

git branch -D $GITHUB_USER-$GITHUB_BRANCH
git checkout -b $GITHUB_USER-$GITHUB_BRANCH
git pull https://github.com/$GITHUB_USER/chapel.git $GITHUB_BRANCH

perf_args="-performance-description $SHORT_NAME -performance-configs default:v,$SHORT_NAME:v -sync-dir-suffix $SHORT_NAME"
perf_args="${perf_args} -numtrials 5 -startdate $START_DATE"
$CWD/nightly -cron ${perf_args} ${nightly_args}
