#!/usr/bin/env bash
#
# Run arkouda testing on a cray-cs

CWD=$(cd $(dirname $0) ; pwd)

export CHPL_TEST_PERF_CONFIG_NAME='16-node-cs'
export CHPL_NIGHTLY_TEST_CONFIG_NAME="perf.cray-cs.arkouda.release"

# setup arkouda
source $CWD/common-arkouda.bash
export ARKOUDA_NUMLOCALES=16

module list

# setup for CS perf (gasnet-large, gnu, 36-core Broadwell)
source $CWD/common-cray-cs.bash
source $CWD/common-perf-cray-cs.bash

# python2 required for chapel 1.23, will not be required with 1.24+
source ~/.setup_python2.bash

module list

export GASNET_PHYSMEM_MAX=83G
export GASNET_ODP_VERBOSE=0
export CHPL_LAUNCHER=slurm-gasnetrun_ibv
nightly_args="${nightly_args} -no-buildcheck"

# workaround for https://github.com/Cray/chapel-private/issues/1598
export CHPL_TEST_TIMEOUT=3000
export CHPL_TEST_NUM_TRIALS=1

test_release
sync_graphs
