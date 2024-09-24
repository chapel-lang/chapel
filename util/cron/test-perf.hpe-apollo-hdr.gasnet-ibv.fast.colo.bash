#!/usr/bin/env bash
#
# Run co-locale performance tests on an HPE Apollo over GASNet with PSHM
# enabled.

CWD=$(cd $(dirname $0) ; pwd)

export CHPL_TEST_PERF_SUBDIR="hpe-apollo"
export CHPL_TEST_PERF_CONFIG_NAME='16-node-apollo-hdr'

source $CWD/common-perf.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="perf.hpe-apollo-hdr.gasnet-ibv.fast.colo"

source $CWD/common-hpe-apollo.bash
source $CWD/common-perf-hpe-apollo-hdr.bash
perf_hpe_apollo_args="-performance-configs gn-ibv-fast:v,gn-ibv-fast-colo:v -perflabel ml- -startdate 09/04/24"

export CHPL_GASNET_SEGMENT=fast
export GASNET_PHYSMEM_MAX="0.90"
export CHPL_RT_LOCALES_PER_NODE=2
export CHPL_GASNET_MORE_CFG_OPTIONS=--enable-pshm

nightly_args="${nightly_args} -no-buildcheck"
perf_args="-performance-description gn-ibv-fast-colo -numtrials 1 -sync-dir-suffix colocales"

# Remove this for production
export CHPL_NIGHTLY_TEST_DIRS=release/examples/benchmarks

$CWD/nightly -cron ${perf_args} ${perf_hpe_apollo_args} ${nightly_args}
