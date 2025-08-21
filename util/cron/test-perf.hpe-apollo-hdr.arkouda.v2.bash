#!/usr/bin/env bash
#
# Run arkouda performance testing on a hpe-apollo with HDR IB

UTIL_CRON_DIR=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)

export ARKOUDA_DEP_DIR=/hpelustre/chapelu/arkouda-deps
export ARKOUDA_SKIP_CHECK_DEPS=true

export CHPL_TEST_ARKOUDA_SKIP_UNIT_TESTS=true
export CHPL_TEST_ARKOUDA_PERF=true

export CHPL_TEST_PERF_CONFIG_NAME='16-node-hpe-apollo-hdr'
export CHPL_NIGHTLY_TEST_CONFIG_NAME="perf.hpe-apollo-hdr.arkouda.v2"

module list

source $UTIL_CRON_DIR/common-perf.bash # sources common.bash
source $UTIL_CRON_DIR/common-hpe-apollo.bash
source $UTIL_CRON_DIR/common-perf-hpe-apollo-hdr.bash
#
# setup arkouda
source $UTIL_CRON_DIR/common-arkouda.bash
source $UTIL_CRON_DIR/common-arkouda-hpe-apollo-hdr.bash


# SETUP FOR V2
export CHPL_TEST_ARKOUDA_PERF=false
export CHPL_TEST_ARKOUDA_PERF_V2=true
unset CHPL_TEST_GEN_ARKOUDA_GRAPHS

export ARKOUDA_NUMLOCALES=16

# on this system, the several tests comes dangerously close to the timeout.
export ARKOUDA_CLIENT_TIMEOUT=1200

export CHPL_GASNET_SEGMENT=fast
export GASNET_PHYSMEM_MAX="0.90"

export CHPL_LLVM_GCC_PREFIX='none'

nightly_args="${nightly_args} -no-buildcheck"

module list

export CHPL_TEST_PERF_SYNC_DIR_SUFFIX=v2
export CHPL_TEST_PERF_DESCRIPTION=nightly-v2
export CHPL_TEST_PERF_CONFIGS="nightly:v,nightly-v2:v"

test_nightly
sync_graphs
