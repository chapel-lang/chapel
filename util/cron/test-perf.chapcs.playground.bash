#!/usr/bin/env bash
#

CWD=$(cd $(dirname $0) ; pwd)

export CHPL_TEST_PERF_CONFIG_NAME='chapcs'

source $CWD/common-perf.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="perf.chapcs.playground"

# Test performance of the blank-ref-intent branch (subset/prereq of arrays work)
GITHUB_USER=benharsh
GITHUB_BRANCH=ben-blank-ref-intent
ARRAYS_SHORT_NAME=arrays
REF_SHORT_NAME=ref
START_DATE=08/30/16
SYNC_SUFFIX=arrays

# Normally we'd just merge the branch in with master here, but it doesn't merge
# cleanly so just check it out instead. This means we actually destroy this
# file out from under ourselves.. but that's "ok" because it's already running
git branch -D $GITHUB_USER-$GITHUB_BRANCH
git remote add $GITHUB_USER https://github.com/$GITHUB_USER/chapel.git
git fetch $GITHUB_USER
git checkout -b $GITHUB_USER-$GITHUB_BRANCH $GITHUB_USER/$GITHUB_BRANCH

perf_args="-performance-description $REF_SHORT_NAME -performance-configs default:v,$REF_SHORT_NAME:v,$ARRAYS_SHORT_NAME:v -sync-dir-suffix $SYNC_SUFFIX"
perf_args="${perf_args} -numtrials 5 -startdate $START_DATE"
$CWD/nightly -cron ${perf_args} ${nightly_args}
