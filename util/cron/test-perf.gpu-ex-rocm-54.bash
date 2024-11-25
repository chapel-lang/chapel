#!/usr/bin/env bash
#
# Run GPU performance tests

UTIL_CRON_DIR=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)
source $UTIL_CRON_DIR/common-native-gpu.bash
source $UTIL_CRON_DIR/common-hpe-cray-ex.bash
source $UTIL_CRON_DIR/common-native-gpu-perf.bash
# make sure this comes after setting SUBDIR (set by native-gpu-perf) and
# CONFIG_NAME
source $UTIL_CRON_DIR/common-perf.bash

# everything we source above will end up sourcing `common.bash` which will then
# source `load-base-deps.bash`. In the system we run this config,
# `load-base-deps.bash` ends up exporting
#   `CHPL_LLVM_CONFIG=(which # llvm-config)`
# If `rocm` module is loaded, rocm's llvm-config takes precedence over our LLVM
# install. We don't want that in this system. So `module load rocm` should
# appear after all the `source`s.
module load rocm/5.4.3  # pin to rocm 5.4.3

export CHPL_COMM=none
export CHPL_LLVM=system
unset CHPL_LLVM_CONFIG  # we need this to avoid warnings
export CHPL_LOCALE_MODEL=gpu
export CHPL_LAUNCHER_PARTITION=bardpeak  # bardpeak is the default queue
export CHPL_GPU=amd  # also detected by default
export CHPL_GPU_ARCH=gfx90a
export CHPL_NIGHTLY_TEST_CONFIG_NAME="perf.gpu-ex-rocm-54"

export CHPL_TEST_PERF_CONFIG_NAME="1-node-mi250x"

nightly_args="${nightly_args} -startdate 04/04/24"

$UTIL_CRON_DIR/nightly -cron ${nightly_args}
