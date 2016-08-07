#!/usr/bin/env bash
#
# Test gasnet configuration with CHPL_LLVM=llvm and pass --llvm flag to
# compiler on linux64.

CWD=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)
source $CWD/common-slurm-gasnet-ibv.bash
source $CWD/common-llvm.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="slurm-gasnet-ibv.llvm"

$CWD/nightly -cron -no-futures -examples ${nightly_args} < /dev/null
