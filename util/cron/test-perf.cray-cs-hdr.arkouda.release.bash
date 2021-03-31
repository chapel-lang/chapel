#!/usr/bin/env bash
#
# Run arkouda testing on a cray-cs with HDR IB

CWD=$(cd $(dirname $0) ; pwd)

export CHPL_TEST_PERF_CONFIG_NAME='16-node-cs-hdr'
export CHPL_NIGHTLY_TEST_CONFIG_NAME="perf.cray-cs-hdr.arkouda.release"

# setup arkouda
source $CWD/common-arkouda.bash
unset ARKOUDA_ZMQ_PATH
unset ARKOUDA_HDF5_PATH
export ARKOUDA_NUMLOCALES=16

module list

# setup for CS perf (gasnet-large, gnu, 128-core Rome)
source $CWD/common-cray-cs.bash
export CHPL_LAUNCHER_PARTITION=rome64Share
export CHPL_TARGET_CPU=none

# python2 required for chapel 1.23, will not be required with 1.24+
source ~/.setup_python2.bash

module list

export GASNET_PHYSMEM_MAX="9/10"
export GASNET_ODP_VERBOSE=0
export CHPL_LAUNCHER=slurm-gasnetrun_ibv
nightly_args="${nightly_args} -no-buildcheck"

test_release
sync_graphs
