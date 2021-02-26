#!/usr/bin/env bash
#
# Test gasnet mpi against hellos on cray-cs.

CWD=$(cd $(dirname $0) ; pwd)
source $CWD/common-slurm-gasnet-cray-cs.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="slurm-gasnet-mpi"

# setup for mpi
module load openmpi/gcc
export CHPL_COMM_SUBSTRATE=mpi
export CHPL_LAUNCHER=slurm-gasnetrun_mpi

$CWD/nightly -cron -hellos ${nightly_args}
