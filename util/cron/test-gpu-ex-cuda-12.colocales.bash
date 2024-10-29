#!/usr/bin/env bash
#
# GPU native testing on a Cray EX for colocales specifically

CWD=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)
source $CWD/common-native-gpu.bash
source $CWD/common-hpe-cray-ex.bash
source $CWD/common-ofi.bash

export SLURM_NETWORK=single_node_vni
export CHPL_RT_LOCALES_PER_NODE=2
module load cuda/12.4

export CHPL_LAUNCHER_PARTITION=griz256
export CHPL_GPU=nvidia
export CHPL_NIGHTLY_TEST_DIRS="gpu/native/multiLocale"

export CHPL_NIGHTLY_TEST_CONFIG_NAME="gpu-ex-cuda-12.colocales"
$CWD/nightly -cron ${nightly_args}
