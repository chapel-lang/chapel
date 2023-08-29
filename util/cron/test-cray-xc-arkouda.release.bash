#!/usr/bin/env bash

CWD=$(cd $(dirname $0) ; pwd)

export CHPL_NIGHTLY_TEST_CONFIG_NAME="cray-xc-arkouda.release"

source $CWD/common.bash

# setup arkouda
source $CWD/common-arkouda.bash
export ARKOUDA_NUMLOCALES=16

export CHPL_TEST_ARKOUDA_PERF=false

module list

# setup for XC perf (ugni, gnu, 28-core broadwell)
module unload $(module -t list 2>&1 | grep PrgEnv-)
module load PrgEnv-gnu
module unload $(module -t list 2>&1 | grep craype-hugepages)
module load craype-hugepages16M
module unload perftools-base
module unload atp
module load craype-x86-cascadelake

module list

export CHPL_LAUNCHER_CONSTRAINT="CL48,192GB"
export CHPL_LAUNCHER_CORES_PER_LOCALE=96
export CHPL_LAUNCHER=slurm-srun
nightly_args="${nightly_args} -no-buildcheck"

test_release
