#!/usr/bin/env bash
#
# Run GPU performance tests

CWD=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)
source $CWD/common-slurm-gasnet-cray-cs.bash
source $CWD/common-native-gpu.bash

export CHPL_GPU=amd
export CHPL_LAUNCHER_PARTITION=amdMI60
export CHPL_GPU_ARCH=gfx906
export CHPL_COMM=none
export CHPL_LLVM=system
export CHPL_NIGHTLY_TEST_CONFIG_NAME="perf.gpu-rocm"

export CHPL_TEST_PERF_CONFIG_NAME="1-node-mi60"
source $CWD/common-native-gpu-perf.bash
# make sure this comes after setting SUBDIR (set by native-gpu-perf) and
# CONFIG_NAME
source $CWD/common-perf.bash
export CHPL_TARGET_CPU=native

# everything we source above will end up sourcing `common.bash` which will then
# source `load-base-deps.bash`. In the system we run this config,
# `load-base-deps.bash` ends up exporting
#   `CHPL_LLVM_CONFIG=(which # llvm-config)`
# If `rocm` module is loaded, rocm's llvm-config takes precedence over our LLVM
# install. We don't want that in this system. So `module load rocm` should
# appear after all the `source`s.
module load rocm

# Use LLVM 14 as we don't support the ROCM bundled LLVM yet.
source /hpcdc/project/chapel/setup_system_llvm.bash 14

nightly_args="${nightly_args} -startdate 07/20/23"

$CWD/nightly -cron ${nightly_args}
