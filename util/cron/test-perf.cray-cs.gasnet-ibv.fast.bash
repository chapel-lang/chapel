#!/usr/bin/env bash
#
# Run performance tests on a cray-cs

CWD=$(cd $(dirname $0) ; pwd)

export CHPL_TEST_PERF_CONFIG_NAME='16-node-cs'

source $CWD/common-perf.bash
export CHPL_TEST_PERF_DIR=/cray/css/users/chapelu/NightlyPerformance/cray-cs/16-node-cs

export CHPL_NIGHTLY_TEST_CONFIG_NAME="perf.cray-cs.gasnet-ibv.fast"

source $CWD/common-cray-cs.bash y
source $CWD/common-perf-cray-cs.bash

# Keep long running tests as a performance issue instead of being reported as a
# correctness timeout -- https://github.com/Cray/chapel-private/issues/443
export CHPL_TEST_TIMEOUT=1800

export CHPL_GASNET_SEGMENT=fast
export GASNET_PHYSMEM_MAX=83G
export CHPL_GASNET_MORE_CFG_OPTIONS=--disable-ibv-odp
nightly_args="${nightly_args} -no-buildcheck"
perf_args="-performance-description gn-ibv-fast -numtrials 1"

$CWD/nightly -cron ${perf_args} ${perf_cray_cs_args} ${nightly_args}
