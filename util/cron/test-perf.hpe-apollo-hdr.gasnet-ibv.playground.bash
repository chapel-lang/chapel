#!/usr/bin/env bash
#
# Run performance tests on an HPE Apollo

CWD=$(cd $(dirname $0) ; pwd)

export CHPL_TEST_PERF_SUBDIR="hpe-apollo"
export CHPL_TEST_PERF_CONFIG_NAME='16-node-apollo-hdr'

source $CWD/common-perf.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="perf.hpe-apollo-hdr.gasnet-ibv.playground"

source $CWD/common-hpe-apollo.bash
source $CWD/common-perf-hpe-apollo-hdr.bash

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
# Currently tests https://github.com/chapel-lang/chapel/pull/25517. We'll
# determine whether to turn that optimization on-by-default.

SKIP_ML_PLAYGROUND=0
if [[ "$SKIP_ML_PLAYGROUND" == "1" ]]; then
  log_info "Skipping testing of the multi-local playground"
  exit
fi

GITHUB_USER=bradcray
GITHUB_BRANCH=enable-localize-domain-opt
SHORT_NAME=localize-domain
START_DATE=07/15/24

git branch -D $GITHUB_USER-$GITHUB_BRANCH
git checkout -b $GITHUB_USER-$GITHUB_BRANCH
git pull https://github.com/$GITHUB_USER/chapel.git $GITHUB_BRANCH

nightly_args="${nightly_args} -no-buildcheck"
# don't use `perf_hpe_apollo_args`, the settings are overwritten here
perf_args="-performance-description $SHORT_NAME -perflabel ml-"
perf_args="${perf_args} -performance-configs gn-ibv-large:v,gn-ibv-fast:v,$SHORT_NAME:v -sync-dir-suffix $SHORT_NAME"
perf_args="${perf_args} -numtrials 1 -startdate $START_DATE"

$CWD/nightly -cron ${perf_args} ${nightly_args}
