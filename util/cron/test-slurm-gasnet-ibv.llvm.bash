#!/usr/bin/env bash
#
# Multi-node, multi-locale testing on a linux64 cluster with slurm-gasnetrun_ibv launcher:
# test gasnet configuration with CHPL_LLVM=llvm and pass --llvm flag to compiler.
# test against "examples"

CWD=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)
source $CWD/common-slurm-gasnet-ibv.bash
source $CWD/common-llvm.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="slurm-gasnet-ibv.llvm"

$CWD/nightly -cron -examples ${nightly_args} < /dev/null
