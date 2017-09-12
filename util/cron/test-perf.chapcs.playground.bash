#!/usr/bin/env bash
#

#
# Instructions: To customize this script to time a given branch, see
# the numbered steps in the comment block below.
#

CWD=$(cd $(dirname $0) ; pwd)

export CHPL_TEST_PERF_CONFIG_NAME='chapcs'

source $CWD/common-perf.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="perf.chapcs.playground"

#
# 0) Update the comment that follows this block to describe what's being tested
# 1) Update GITHUB_USER to the GitHub username owning the branch
# 2) Update GITHUB_BRANCH to the name of the GitHub branch under that user
# 3) Update SHORT_NAME to be a short name that will be used in graph generation
# 4) Update START_DATE to be today
#

# Test performance of inlining all direct iters (not just those with 1 yield)
GITHUB_USER=ronawho
GITHUB_BRANCH=inline-all-direct-iters
SHORT_NAME=inline-all-direct-iters
START_DATE=09/11/17

git branch -D $GITHUB_USER-$GITHUB_BRANCH
git checkout -b $GITHUB_USER-$GITHUB_BRANCH
git pull https://github.com/$GITHUB_USER/chapel.git $GITHUB_BRANCH

perf_args="-performance-description $SHORT_NAME -performance-configs default:v,$SHORT_NAME:v -sync-dir-suffix $SHORT_NAME"
perf_args="${perf_args} -numtrials 5 -startdate $START_DATE"
$CWD/nightly -cron ${perf_args} ${nightly_args}
