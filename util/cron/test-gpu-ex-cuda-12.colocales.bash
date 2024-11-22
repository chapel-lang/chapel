#!/usr/bin/env bash
#
# GPU native testing on a Cray EX for colocales specifically

UTIL_CRON_DIR=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)
source $UTIL_CRON_DIR/common-native-gpu.bash
source $UTIL_CRON_DIR/common-hpe-cray-ex.bash
source $UTIL_CRON_DIR/common-ofi.bash

export SLURM_NETWORK=single_node_vni
export CHPL_RT_LOCALES_PER_NODE=2
module load cuda/12.4

export CHPL_LAUNCHER_PARTITION=griz256
export CHPL_GPU=nvidia
export CHPL_NIGHTLY_TEST_DIRS="gpu/native/multiLocale"

export CHPL_NIGHTLY_TEST_CONFIG_NAME="gpu-ex-cuda-12.colocales"
$UTIL_CRON_DIR/nightly -cron ${nightly_args}
