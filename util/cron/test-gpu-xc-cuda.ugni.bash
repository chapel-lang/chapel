#!/usr/bin/env bash
#
# Build Arkouda on a cray-xc with GPU locale model

UTIL_CRON_DIR=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)

source $UTIL_CRON_DIR/common-native-gpu.bash

# Setup for GPU:
module load cudatoolkit
export CHPL_TARGET_COMPILER=llvm
export CHPL_LLVM=bundled
export CHPL_TARGET_CPU=native
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


export CHPL_NIGHTLY_TEST_CONFIG_NAME="gpu-xc-cuda.ugni"
$UTIL_CRON_DIR/nightly -cron ${nightly_args}