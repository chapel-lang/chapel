#!/usr/bin/env bash
#

#
# Instructions: To customize this script to time a given branch, see
# the numbered steps in the comment block below.
#
# The graphs will be at:
#    https://chapel-lang.org/perf/chapcs/$SHORT_NAME
#

UTIL_CRON_DIR=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)

export CHPL_TEST_PERF_CONFIG_NAME='chapcs'

source $UTIL_CRON_DIR/common-perf.bash
source $UTIL_CRON_DIR/common-playground.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="perf.chapcs.playground"

#
# 0) Update the comment that follows this block to describe what's being tested
# 1) Update GITHUB_USER to the GitHub username owning the branch
# 2) Update GITHUB_BRANCH to the name of the GitHub branch under that user
# 3) Update SHORT_NAME to be a short name that will be used in graph generation
# 4) Update START_DATE to be today, using the format mm/dd/yy
#

# Test performance with the latest qthreads release.
GITHUB_USER=insertinterestingnamehere
GITHUB_BRANCH=qthread
SHORT_NAME=qthread122
START_DATE=02/21/25

set -e
checkout_branch $GITHUB_USER $GITHUB_BRANCH
set +e

perf_args="-performance-description $SHORT_NAME -performance-configs default:v,$SHORT_NAME:v -sync-dir-suffix $SHORT_NAME"
perf_args="${perf_args} -numtrials 1 -startdate $START_DATE"
$UTIL_CRON_DIR/nightly -cron ${perf_args} ${nightly_args}
