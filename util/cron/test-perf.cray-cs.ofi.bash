#!/usr/bin/env bash
#
# Run performance tests with comm=ofi on a cray-cs system.

CWD=$(cd $(dirname $0) ; pwd)

export CHPL_TEST_PERF_CONFIG_NAME='16-node-cs'

source $CWD/common-perf.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="perf.cray-cs.ofi"

module load gcc
module load python/2.7.6
module load pmix

export CHPL_HOST_PLATFORM=cray-cs
export CHPL_COMM=ofi
export CHPL_COMM_OFI_OOB=slurm-pmi2
export SLURM_MPI_TYPE=pmix
export CHPL_RT_MAX_HEAP_SIZE=83G
export CHPL_LAUNCHER=slurm-srun
export CHPL_LAUNCHER_PARTITION=bdw18
export CHPL_TARGET_CPU=broadwell

source /cray/css/users/chapelu/setup_libfabric.bash || return 1

nightly_args="${nightly_args} -no-buildcheck"

perf_args="-performance-description ofi -performance-configs gn-ibv-large,gn-ibv-fast:v,gn-mpi"
perf_args="${perf_args} -performance -perflabel ml- -numtrials 1 -startdate 12/01/19"

$CWD/nightly -cron ${perf_args} ${nightly_args}
