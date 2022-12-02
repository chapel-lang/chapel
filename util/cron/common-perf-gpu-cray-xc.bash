#!/usr/bin/env bash

CWD=$(cd $(dirname $0) ; pwd)

source $CWD/common.bash

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

# setup for XC perf (ugni, gnu, 28-core broadwell)
module unload $(module -t list 2>&1 | grep PrgEnv-)
module load PrgEnv-gnu
module unload $(module -t list 2>&1 | grep craype-hugepages)
module load craype-hugepages16M
module unload perftools-base
module unload atp
module load craype-x86-cascadelake
