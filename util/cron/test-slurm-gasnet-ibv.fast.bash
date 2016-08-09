#!/usr/bin/env bash
#
# Multi-node, multi-locale testing on a linux64 cluster with slurm-gasnetrun_ibv launcher:
# test gasnet (segment fast) against full suite.

CWD=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)
source $CWD/common-slurm-gasnet-ibv.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="slurm-gasnet-ibv.fast"

export GASNET_QUIET=Y

# Test a GASNet compile using the fast segment
export CHPL_GASNET_SEGMENT=fast

$CWD/nightly -cron ${nightly_args} < /dev/null
