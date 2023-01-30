#!/usr/bin/env bash
#
# Run GPU performance tests

CWD=$(cd $(dirname $0) ; pwd)
source $CWD/common-native-gpu.bash
export CHPL_COMM=none
export CHPL_NIGHTLY_TEST_DIRS="gpu/native/array_on_device"
export CHPL_GPU_MEM_STRATEGY=array_on_device

export CHPL_TEST_PERF_CONFIG_NAME='gpu'
source $CWD/common-perf.bash
export CHPL_NIGHTLY_TEST_CONFIG_NAME="perf.cray-cs-hdr.gpu-array-on-device"

nightly_args="${nightly_args} -performance -perflabel gpu- -numtrials 5 -startdate 07/15/22"
$CWD/nightly -cron ${nightly_args}
