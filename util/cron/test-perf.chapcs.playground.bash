#!/usr/bin/env bash
#

#
# Instructions: To customize this script to time a given branch, see
# the numbered steps in the comment block below.
#
# The graphs will be at:
#    https://chapel-lang.org/perf/chapcs/$SHORT_NAME
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
# 4) Update START_DATE to be today, using the format mm/dd/yy
#

# Test performance of getting arrays to use copy init
GITHUB_USER=mppf
GITHUB_BRANCH=array-coerce
SHORT_NAME=array-coerce
START_DATE=05/18/20

git branch -D $GITHUB_USER-$GITHUB_BRANCH
git checkout -b $GITHUB_USER-$GITHUB_BRANCH
git pull https://github.com/$GITHUB_USER/chapel.git $GITHUB_BRANCH

perf_args="-performance-description $SHORT_NAME -performance-configs default:v,$SHORT_NAME:v -sync-dir-suffix $SHORT_NAME"
perf_args="${perf_args} -numtrials 1 -startdate $START_DATE"
$CWD/nightly -cron ${perf_args} ${nightly_args} -compopts -senablePostfixBangChecks
