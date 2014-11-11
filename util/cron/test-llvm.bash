#!/usr/bin/env bash
#
# Test default configuration with CHPL_LLVM=llvm and pass --llvm flag to
# compiler on linux64.

CWD=$(cd $(dirname $0) ; pwd)
source $CWD/common.bash
source $CWD/common-llvm.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="llvm"

$CWD/nightly -cron -examples ${nightly_args}
