#!/usr/bin/env bash
#
# Run GPU performance tests

CWD=$(cd $(dirname $0) ; pwd)
source $CWD/common-native-gpu.bash
module load cudatoolkit

export CHPL_GPU=nvidia
export CHPL_LAUNCHER_PARTITION=stormP100

export CHPL_COMM=none

export CHPL_TEST_PERF_CONFIG_NAME='gpu'
source $CWD/common-perf.bash
export CHPL_NIGHTLY_TEST_CONFIG_NAME="perf.gpu-cuda"

nightly_args="${nightly_args} -performance -perflabel gpu- -numtrials 5 -startdate 07/15/22"
$CWD/nightly -cron ${nightly_args}
