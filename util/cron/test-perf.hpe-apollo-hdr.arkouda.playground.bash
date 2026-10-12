#!/usr/bin/env bash
#
# Run arkouda performance testing on a hpe-apollo with HDR IB

UTIL_CRON_DIR=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)

export ARKOUDA_DEP_DIR=/hpelustre/chapelu/arkouda-deps
export ARKOUDA_SKIP_CHECK_DEPS=true

export CHPL_TEST_ARKOUDA_SKIP_UNIT_TESTS=true
export CHPL_TEST_ARKOUDA_PERF=true

export CHPL_TEST_PERF_CONFIG_NAME='16-node-hpe-apollo-hdr'
export CHPL_NIGHTLY_TEST_CONFIG_NAME="perf.hpe-apollo-hdr.arkouda.playground"

module list

source $UTIL_CRON_DIR/common-perf.bash # sources common.bash
source $UTIL_CRON_DIR/common-playground.bash
source $UTIL_CRON_DIR/common-hpe-apollo.bash
source $UTIL_CRON_DIR/common-perf-hpe-apollo-hdr.bash


#
# 0) Update the comment that follows this block to describe what's being tested
# 1) Adjust TEST_NIGHTLY to be 1 if you want to test the nightly branch
#    By default, this job will test the latest Chapel release
# 2) Follow these steps to adjust the Chapel branch being tested
#    Don't change it if you want to test the release
#   0) Set TEST_NIGHTLY to 1
#   1) Update GITHUB_USER to the GitHub username owning the branch
#   2) Update GITHUB_BRANCH to the name of the GitHub branch under that user
# 3) Follow these steps to adjust the location the graphs will be stored
#   0) Update SHORT_NAME to be a short name that will be used in graph generation
#   1) Update START_DATE to be today, using the format mm/dd/yy
# 4) Follow these steps to adjust the arkouda branch being tested
#   0) Update ARKOUDA_URL to the GitHub URL of the arkouda repo
#   1) Update ARKOUDA_BRANCH to the name of the branch under that repo
#
# When the multi-local playground is not used, set `SKIP_ARKOUDA_PLAYGROUND=1
#

# Test what happens to performance if we disable the
# --interprocedural-alias-analysis pass by default

SKIP_ARKOUDA_PLAYGROUND=0
if [[ "$SKIP_ARKOUDA_PLAYGROUND" == "1" ]]; then
  log_info "Skipping testing of the arkouda playground"
  exit
fi

TEST_NIGHTLY=1

GITHUB_USER=bradcray
GITHUB_BRANCH=no-noAliasSets2

SHORT_NAME=noAliasAnalysis
START_DATE=2/19/26

export ARKOUDA_URL=https://github.com/Bears-R-Us/arkouda.git
export ARKOUDA_BRANCH=main

if [[ "$TEST_NIGHTLY" == "1" ]]; then
  set -e
  checkout_branch $GITHUB_USER $GITHUB_BRANCH
  set +e
else
  if [[ "$GITHUB_USER" != "chapel-lang" || "$GITHUB_BRANCH" != "main" ]]; then
    log_error "To test a non-release branch, set TEST_NIGHTLY=1 and update GITHUB_USER and GITHUB_BRANCH"
    exit 1
  fi
fi


#
# setup arkouda
source $UTIL_CRON_DIR/common-arkouda.bash
source $UTIL_CRON_DIR/common-arkouda-hpe-apollo-hdr.bash

export ARKOUDA_NUMLOCALES=16

# on this system, the several tests comes dangerously close to the timeout.
export ARKOUDA_CLIENT_TIMEOUT=1200

export CHPL_GASNET_SEGMENT=fast
export GASNET_PHYSMEM_MAX="0.90"

export CHPL_LLVM_GCC_PREFIX='none'

nightly_args="${nightly_args} -no-buildcheck"

module list

export CHPL_TEST_PERF_SYNC_DIR_SUFFIX=$SHORT_NAME
export CHPL_TEST_PERF_DESCRIPTION=$SHORT_NAME
export CHPL_TEST_PERF_CONFIGS="release:v,nightly:v,$SHORT_NAME:v"

if [[ "$TEST_NIGHTLY" == "1" ]]; then
  test_nightly
else
  test_release
fi
sync_graphs
