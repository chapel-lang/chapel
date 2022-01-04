#!/usr/bin/env bash
#
# Test incremental compilation on examples only,
# using no-local configuration on linux64.

CWD=$(cd $(dirname $0) ; pwd)
source $CWD/common.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="no-local.incr"
export CHPL_SYSTEM_PREDIFF=$CHPL_HOME/util/test/prediff-for-incremental-warning

# incremental compilation not currently supported with LLVM
export CHPL_LLVM=none

$CWD/nightly -cron -no-local -examples -compopts --incremental
