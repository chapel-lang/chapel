#!/usr/bin/env bash
#
# GPU native testing on a Cray EX

CWD=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)
source $CWD/common-native-gpu.bash
source $CWD/common-hpe-cray-ex.bash

module load rocm/6.2.0  # pin to rocm 6.2.0

export CHPL_COMM=ofi
export CHPL_LLVM=bundled
unset CHPL_LLVM_CONFIG  # we need this to avoid warnings
export CHPL_LOCALE_MODEL=gpu
export CHPL_LAUNCHER_PARTITION=bardpeak  # bardpeak is the default queue
export CHPL_GPU=amd  # also detected by default
export CHPL_GPU_ARCH=gfx90a

export CHPL_NIGHTLY_TEST_CONFIG_NAME="gpu-ex-rocm-62.ofi"
$CWD/nightly -cron ${nightly_args}
