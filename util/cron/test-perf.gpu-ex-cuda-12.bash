#!/usr/bin/env bash
#
# Run GPU performance testing on a Cray EX

CWD=$(cd $(dirname $0) ; pwd)
source $CWD/common-native-gpu.bash
source $CWD/common-hpe-cray-ex.bash

module load cuda/12.4

export CHPL_LLVM=system
export CHPL_COMM=none
export CHPL_LOCALE_MODEL=gpu
export CHPL_LAUNCHER_PARTITION=griz256
export CHPL_GPU=nvidia  # amd is detected automatically

export CHPL_NIGHTLY_TEST_CONFIG_NAME="perf.gpu-ex-cuda-12"

export CHPL_TEST_PERF_CONFIG_NAME="1-node-a100" # pinoak has ampere GPUs
source $CWD/common-native-gpu-perf.bash
# make sure this comes after setting SUBDIR (set by native-gpu-perf) and
# CONFIG_NAME
source $CWD/common-perf.bash

nightly_args="${nightly_args} -startdate 04/04/24"

$CWD/nightly -cron ${nightly_args}
