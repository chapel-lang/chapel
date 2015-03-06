#!/usr/bin/env bash
#
# Test gasnet configuration with CHPL_LLVM=llvm and pass --llvm flag to
# compiler on linux64.

CWD=$(cd $(dirname $0) ; pwd)
source $CWD/common-gasnet.bash
source $CWD/common-llvm.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="gasnet.llvm"

$CWD/nightly -cron -examples ${nightly_args}
