#!/usr/bin/env bash
#
# Build Arkouda on a cray-xc with GPU locale model

CWD=$(cd $(dirname $0) ; pwd)

# Setup for GPU:
source /cray/css/users/chapelu/setup_system_llvm.bash $LLVM_VERSION
module load cudatoolkit
export CHPL_TARGET_COMPILER=llvm
export CHPL_LLVM=system
export CHPL_TARGET_CPU=native
export CHPL_COMM=none
export CHPL_LOCALE_MODEL=gpu
export CHPL_TEST_GPU=true
export CHPL_LAUNCHER_CONSTRAINT=BW18
export CHPL_LAUNCHER="slurm-srun"

# For some reason needed to not fail when building Arkouda with
# CHPL_LOCALE_MODEL=gpu:
module unload cce
module load PrgEnv-gnu

# setup arkouda
export CHPL_NIGHTLY_TEST_CONFIG_NAME="cray-xc-gpu-arkouda"
export CHPL_TEST_ARKOUDA_PERF=false
source $CWD/common-arkouda.bash

# List of Arkouda server modules we exempt from testing (that goal is to
# eventually have this be an empty list).
export CHPL_TEST_ARKOUDA_DISABLE_MODULES=ArraySetopsMsg:KExtremeMsg:ArgSortMsg:SegmentedMsg:DataFrameIndexingMsg:UniqueMsg:In1dMsg:SortMsg:ReductionMsg:EfuncMsg:HDF5Msg:EncodingMsg
export CHPL_TEST_ARKOUDA_STOP_AFTER_BUILD="true"

module list
nightly_args="${nightly_args} -no-buildcheck"

test_nightly
