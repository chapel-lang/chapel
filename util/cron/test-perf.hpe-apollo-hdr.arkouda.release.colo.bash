#!/usr/bin/env bash
#
# Run arkouda performance testing on a hpe-apollo with HDR IB using 2 colocales
# per node

UTIL_CRON_DIR=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)

export ARKOUDA_DEP_DIR=/hpelustre/chapelu/arkouda-deps
export ARKOUDA_SKIP_CHECK_DEPS=true

export CHPL_TEST_ARKOUDA_SKIP_UNIT_TESTS=true
export CHPL_TEST_ARKOUDA_PERF=true

export CHPL_TEST_PERF_CONFIG_NAME='16-node-hpe-apollo-hdr'
export CHPL_NIGHTLY_TEST_CONFIG_NAME="perf.hpe-apollo-hdr.arkouda.release.colo"

module list

source $UTIL_CRON_DIR/common-perf.bash # sources common.bash
source $UTIL_CRON_DIR/common-hpe-apollo.bash
source $UTIL_CRON_DIR/common-perf-hpe-apollo-hdr.bash
#
# setup arkouda
source $UTIL_CRON_DIR/common-arkouda.bash
source $UTIL_CRON_DIR/common-arkouda-hpe-apollo-hdr.bash

export CHPL_RT_LOCALES_PER_NODE=2
export ARKOUDA_NUMLOCALES=32

# on this system, the several tests comes dangerously close to the timeout.
export ARKOUDA_CLIENT_TIMEOUT=1200

export CHPL_GASNET_SEGMENT=fast
export GASNET_PHYSMEM_MAX="0.90"

export CHPL_LLVM_GCC_PREFIX='none'

setup_release true

nightly_args="${nightly_args} -no-buildcheck"

module list

# normally, the syncdir suffix is an arg to nightly. But arkouda doesn't sync
# graphs through nightly. It has its own sync_graphs
export CHPL_TEST_PERF_SYNC_DIR_SUFFIX=colocales
export CHPL_TEST_PERF_DESCRIPTION=release-colo
export CHPL_TEST_PERF_CONFIGS="release,nightly,release-colo:v,nightly-colo:v"

test_release
sync_graphs
