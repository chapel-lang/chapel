#!/usr/bin/env bash
#
# Multi-node, multi-locale testing on a cray-cs cluster with slurm-gasnetrun_ibv
# launcher:
# test gasnet (segment fast) against "examples"

CWD=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)
source $CWD/common-slurm-gasnet-cray-cs.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="slurm-gasnet-ibv.fast"

export CHPL_COMM_SUBSTRATE=ibv
export CHPL_LAUNCHER=slurm-gasnetrun_ibv

# Test a GASNet compile using the fast segment
export CHPL_GASNET_SEGMENT=fast

$CWD/nightly -cron -examples ${nightly_args} < /dev/null
