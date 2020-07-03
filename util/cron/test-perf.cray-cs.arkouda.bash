#!/usr/bin/env bash
#
# Run arkouda testing on a cray-cs

CWD=$(cd $(dirname $0) ; pwd)

export CHPL_TEST_PERF_CONFIG_NAME='16-node-cs'
export CHPL_NIGHTLY_TEST_CONFIG_NAME="perf.cray-cs.arkouda"

# setup arkouda
source $CWD/common-arkouda.bash
export ARKOUDA_NUMLOCALES=16

# setup for CS perf (gasnet-large, gnu, 36-core Broadwell)
source $CWD/common-cray-cs.bash
source $CWD/common-perf-cray-cs.bash
export GASNET_PHYSMEM_MAX=83G
export GASNET_ODP_VERBOSE=0
export CHPL_LAUNCHER=slurm-gasnetrun_ibv
nightly_args="${nightly_args} -no-buildcheck"

# Skip setops for master testing (fragmentation causes timeout/oom)
export CHPL_TEST_ARKOUDA_BENCHMARKS='stream argsort coargsort gather scatter reduce scan noop'
test_release
unset CHPL_TEST_ARKOUDA_BENCHMARKS
test_master
sync_graphs
