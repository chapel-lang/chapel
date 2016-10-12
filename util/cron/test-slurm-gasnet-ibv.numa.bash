#!/usr/bin/env bash
#
# Multi-node, multi-locale testing on a linux64 cluster with slurm-gasnetrun_ibv launcher:
# test numa locale model with comm=gasnet against "examples"

CWD=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)
source $CWD/common-slurm-gasnet-ibv.bash

export CHPL_LOCALE_MODEL=numa

export CHPL_NIGHTLY_TEST_CONFIG_NAME="slurm-gasnet-ibv.numa"

$CWD/nightly -cron -examples ${nightly_args} < /dev/null
