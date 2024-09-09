#!/usr/bin/env bash
#
# Run performance tests on an HPE Cray EX

CWD=$(cd $(dirname $0) ; pwd)

export CHPL_TEST_PERF_SUBDIR="hpe-cray-ex"
export CHPL_TEST_PERF_CONFIG_NAME='1-node-hpe-cray-ex'

source $CWD/common-perf.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="perf.hpe-cray-ex-ofi.1node"

source $CWD/common-ofi.bash || \
  ( echo "Could not set up comm=ofi testing." && exit 1 )
source $CWD/common-hpe-cray-ex.bash

export CHPL_RT_COMM_OFI_EXPECTED_PROVIDER="cxi"
export CHPL_RT_MAX_HEAP_SIZE=16g

nightly_args="${nightly_args} -no-buildcheck"
perf_args="-performance -perflabel perf -numtrials 1"
perf_hpe_cray_ex_args="-startdate 06/25/24"

$CWD/nightly -cron ${perf_args} ${perf_hpe_cray_ex_args} ${nightly_args}
