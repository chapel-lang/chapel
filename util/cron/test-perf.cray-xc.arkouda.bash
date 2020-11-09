#!/usr/bin/env bash
#
# Run arkouda testing on a cray-xc

CWD=$(cd $(dirname $0) ; pwd)

export CHPL_TEST_PERF_CONFIG_NAME='16-node-xc'
export CHPL_NIGHTLY_TEST_CONFIG_NAME="perf.cray-xc.arkouda"

# setup arkouda
source $CWD/common-arkouda.bash
export ARKOUDA_NUMLOCALES=16

module list

# setup for XC perf (ugni, gnu, 28-core broadwell)
module unload $(module -t list 2>&1 | grep PrgEnv-)
module load PrgEnv-gnu
module unload $(module -t list 2>&1 | grep craype-hugepages)
module load craype-hugepages16M
module unload perftools-base
module unload atp

module list

export CHPL_LAUNCHER_CONSTRAINT=BW28
export CHPL_LAUNCHER_CORES_PER_LOCALE=56
export CHPL_LAUNCHER=slurm-srun
nightly_args="${nightly_args} -no-buildcheck"

test_nightly
sync_graphs
