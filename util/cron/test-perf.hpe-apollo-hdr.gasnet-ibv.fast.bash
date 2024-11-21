#!/usr/bin/env bash
#
# Run performance tests on an HPE Apollo

CWD=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)

export CHPL_TEST_PERF_SUBDIR="hpe-apollo"
export CHPL_TEST_PERF_CONFIG_NAME='16-node-apollo-hdr'

source $CWD/common-perf.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="perf.hpe-apollo-hdr.gasnet-ibv.fast"

source $CWD/common-hpe-apollo.bash
source $CWD/common-perf-hpe-apollo-hdr.bash

export CHPL_GASNET_SEGMENT=fast
export GASNET_PHYSMEM_MAX="0.90"

nightly_args="${nightly_args} -no-buildcheck"
perf_args="-performance-description gn-ibv-fast -numtrials 1"

$CWD/nightly -cron ${perf_args} ${perf_hpe_apollo_args} ${nightly_args}
