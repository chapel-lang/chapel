#!/usr/bin/env bash
#
# Run arkouda testing on a cray-cs with HDR IB

CWD=$(cd $(dirname $0) ; pwd)

export CHPL_TEST_PERF_CONFIG_NAME='16-node-cs-hdr'
export CHPL_NIGHTLY_TEST_CONFIG_NAME="perf.cray-cs-hdr.arkouda"

# setup arkouda
source $CWD/common-arkouda.bash
export ARKOUDA_NUMLOCALES=16

module list

# setup for CS perf (gasnet-large, gnu, 128-core Rome)
source $CWD/common-cray-cs.bash
export CHPL_LAUNCHER_PARTITION=rome64
export CHPL_TARGET_CPU=none

module list

export GASNET_PHYSMEM_MAX="9/10"
export GASNET_ODP_VERBOSE=0
export CHPL_LAUNCHER=slurm-gasnetrun_ibv
nightly_args="${nightly_args} -no-buildcheck"

test_nightly
sync_graphs
