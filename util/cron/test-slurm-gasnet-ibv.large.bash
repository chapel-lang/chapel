#!/usr/bin/env bash
#
# Multi-node, multi-locale testing on a linux64 cluster with slurm-gasnetrun_ibv launcher:
# test gasnet (segment large) against "examples"

CWD=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)
source $CWD/common-slurm-gasnet-ibv.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="slurm-gasnet-ibv.large"

# Test a GASNet compile using the large segment
export CHPL_GASNET_SEGMENT=large

$CWD/nightly -cron -examples ${nightly_args} < /dev/null
