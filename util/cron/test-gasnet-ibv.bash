#!/usr/bin/env bash
#
# Test gasnet ibv against hellos on linux64.

CWD=$(cd $(dirname $0) ; pwd)
source $CWD/common.bash

# setup for ibv
export CHPL_COMM=gasnet
export CHPL_COMM_SUBSTRATE=ibv
export GASNET_PHYSMEM_MAX=16G
export GASNET_PHYSMEM_NOPROBE=1
export GASNET_ODP_VERBOSE=0

export CHPL_TEST_NUM_LOCALES_AVAILABLE=$SLURM_NNODES

nightly_args="${nightly_args} -no-buildcheck"

export CHPL_NIGHTLY_TEST_CONFIG_NAME="gasnet-ibv"

$CWD/nightly -cron -hellos ${nightly_args}
