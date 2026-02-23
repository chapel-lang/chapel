#!/usr/bin/env bash
#
# Run performance tests on an HPE Apollo

UTIL_CRON_DIR=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)

export CHPL_TEST_PERF_SUBDIR="hpe-apollo"
export CHPL_TEST_PERF_CONFIG_NAME='16-node-apollo-hdr'

source $UTIL_CRON_DIR/common-perf.bash
source $UTIL_CRON_DIR/common-playground.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="perf.hpe-apollo-hdr.gasnet-ibv.playground"

source $UTIL_CRON_DIR/common-hpe-apollo.bash
source $UTIL_CRON_DIR/common-perf-hpe-apollo-hdr.bash

export GASNET_PHYSMEM_MAX="0.90"

#
# 0) Update the comment that follows this block to describe what's being tested
# 1) Update GITHUB_USER to the GitHub username owning the branch
# 2) Update GITHUB_BRANCH to the name of the GitHub branch under that user
# 3) Update SHORT_NAME to be a short name that will be used in graph generation
# 4) Update START_DATE to be today, using the format mm/dd/yy
#
# When the multi-local playground is not used, set `SKIP_ML_PLAYGROUND=1
#

SKIP_ML_PLAYGROUND=0
if [[ "$SKIP_ML_PLAYGROUND" == "1" ]]; then
  log_info "Skipping testing of the multi-local playground"
  exit
fi

# Test what happens to performance if we disable the
# --interprocedural-alias-analysis pass by default

GITHUB_USER=bradcray
GITHUB_BRANCH=no-noAliasSets2
SHORT_NAME=noAliasAnalysis
START_DATE=2/19/26

set -e
checkout_branch $GITHUB_USER $GITHUB_BRANCH
set +e

nightly_args="${nightly_args} -no-buildcheck"
# don't use `perf_hpe_apollo_args`, the settings are overwritten here
perf_args="-performance-description $SHORT_NAME -perflabel ml-"
perf_args="${perf_args} -performance-configs gn-ibv-large:v,gn-ibv-fast:v,$SHORT_NAME:v -sync-dir-suffix $SHORT_NAME"
perf_args="${perf_args} -numtrials 1 -startdate $START_DATE"

$UTIL_CRON_DIR/nightly -cron ${perf_args} ${nightly_args}

