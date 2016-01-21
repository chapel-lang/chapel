#!/usr/bin/env bash
#

CWD=$(cd $(dirname $0) ; pwd)

export CHPL_TEST_PERF_CONFIG_NAME='chapcs'

source $CWD/common-perf.bash
source $CWD/common-numa.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="perf.chapcs.numa-playground"

export CHPL_TEST_TIMEOUT=1200


# Test performance of numa with work stealing turned off
#
# Graph the default config and this config side by side to make comparison
# easy, but sync to a different direction so the default chap04 graphs don't
# have multiple configurations.
GITHUB_USER=ronawho
GITHUB_BRANCH=numa-no-work-steal
SHORT_NAME=numa-no-steal
START_DATE=01/20/16

git branch -D $GITHUB_USER-$GITHUB_BRANCH
git checkout -b $GITHUB_USER-$GITHUB_BRANCH
git pull https://github.com/$GITHUB_USER/chapel.git $GITHUB_BRANCH

perf_args="-performance-description $SHORT_NAME -performance-configs default:v,numa:v,$SHORT_NAME:v -sync-dir-suffix $SHORT_NAME"
perf_args="${perf_args} -numtrials 1 -startdate $START_DATE"
$CWD/nightly -cron ${perf_args} ${nightly_args}
