#!/usr/bin/env bash
#
# Run performance tests on an HPE Cray EX

CWD=$(cd $(dirname $0) ; pwd)

source $CWD/common.bash
source $CWD/common-ofi.bash || \
  ( echo "Could not set up comm=ofi testing." && exit 1 )
source $CWD/common-hpe-cray-ex.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="hpe-cray-ex-ofi"
export CHPL_RT_COMM_OFI_EXPECTED_PROVIDER="cxi"
export CHPL_RT_MAX_HEAP_SIZE=16g

export CHPL_TEST_PERF_SUBDIR="hpe-ex"
export CHPL_TEST_PERF_CONFIG_NAME='16-node-hpe-ex'
export CHPL_NIGHTLY_TEST_CONFIG_NAME="perf.hpe-ex.ofi"

nightly_args="${nightly_args} -no-buildcheck"
perf_args="-numtrials 1"

$CWD/nightly -cron ${perf_args} ${perf_hpe_apollo_args} ${nightly_args}
