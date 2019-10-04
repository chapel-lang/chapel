#!/usr/bin/env bash
#
# Run performance tests on a cray-cs

CWD=$(cd $(dirname $0) ; pwd)

export CHPL_TEST_PERF_CONFIG_NAME='16 node CS'

source $CWD/common-perf.bash
export CHPL_TEST_PERF_DIR=/cray/css/users/chapelu/NightlyPerformance/cray-cs/16-node-cs

export CHPL_NIGHTLY_TEST_CONFIG_NAME="perf.cray-cs.gasnet-mpi"

module load gcc
module load openmpi/gcc

export CHPL_HOST_PLATFORM=cray-cs
export CHPL_COMM_SUBSTRATE=mpi
export GASNET_QUIET=y
export CHPL_LAUNCHER=slurm-gasnetrun_mpi
export CHPL_LAUNCHER_PARTITION=bdw18
export CHPL_TARGET_CPU=broadwell
nightly_args="${nightly_args} -no-buildcheck"

perf_args="-performance-description gn-mpi -performance-configs gn-ibv-large:v,gn-ibv-fast:v,gn-mpi"
perf_args="${perf_args} -performance -perflabel ml- -numtrials 1 -startdate 07/01/19"

$CWD/nightly -cron ${perf_args} ${nightly_args}
