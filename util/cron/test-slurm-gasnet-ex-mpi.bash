#!/usr/bin/env bash
#
# Test gasnet-ex mpi against hellos on cray-cs.

CWD=$(cd $(dirname $0) ; pwd)
source $CWD/common-slurm-gasnet-cray-cs.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="slurm-gasnet-ex-mpi"

# setup for mpi
module load openmpi/gcc
export CHPL_COMM_SUBSTRATE=mpi

# Test GASNet EX
export CHPL_GASNET_VERSION=ex

$CWD/nightly -cron -hellos ${nightly_args}
