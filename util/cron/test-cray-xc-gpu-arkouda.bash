#!/usr/bin/env bash
#
# Build Arkouda on a cray-xc with GPU locale model

CWD=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)

source $CWD/common.bash

# Setup for GPU:
source /hpcdc/project/chapel/setup_system_llvm.bash $LLVM_VERSION
module load cudatoolkit
export CHPL_TARGET_COMPILER=llvm
export CHPL_LLVM=system
export CHPL_TARGET_CPU=native
export CHPL_COMM=none
export CHPL_LOCALE_MODEL=gpu
export CHPL_TEST_GPU=true
export CHPL_LAUNCHER_CONSTRAINT=BW18
export CHPL_LAUNCHER="slurm-srun"

# setup for XC perf (ugni, gnu, 28-core broadwell)
module unload $(module -t list 2>&1 | grep PrgEnv-)
module load PrgEnv-gnu
module unload $(module -t list 2>&1 | grep craype-hugepages)
module load craype-hugepages16M
module unload perftools-base
module unload atp
module load craype-x86-cascadelake

# setup arkouda
export CHPL_NIGHTLY_TEST_CONFIG_NAME="cray-xc-gpu-arkouda"
export CHPL_TEST_ARKOUDA_PERF=false
source $CWD/common-arkouda.bash

# List of Arkouda server modules we exempt from testing (that goal is to
# eventually have this be an empty list).
export CHPL_TEST_ARKOUDA_DISABLE_MODULES=In1dMsg:HDF5Msg:HDF5Msg_LEGACY:HDF5MultiDim
export CHPL_TEST_ARKOUDA_STOP_AFTER_BUILD="true"

module list
nightly_args="${nightly_args} -no-buildcheck"

test_nightly
