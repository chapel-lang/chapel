#!/usr/bin/env bash
#
# Run performance tests on an HPE Apollo

UTIL_CRON_DIR=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)

export CHPL_TEST_PERF_SUBDIR="hpe-apollo"
export CHPL_TEST_PERF_CONFIG_NAME='16-node-apollo-hdr'

source $UTIL_CRON_DIR/common-perf.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="perf.hpe-apollo-hdr.gasnet-ibv.large"

source $UTIL_CRON_DIR/common-hpe-apollo.bash
source $UTIL_CRON_DIR/common-perf-hpe-apollo-hdr.bash

export GASNET_PHYSMEM_MAX="0.90"
nightly_args="${nightly_args} -no-buildcheck"
perf_args="-performance-description gn-ibv-large -numtrials 1"

$UTIL_CRON_DIR/nightly -cron ${perf_args} ${perf_hpe_apollo_args} ${nightly_args}
