#!/usr/bin/env bash
#
# Test incremental compilation on examples only,
# using default linux64 configuration.

CWD=$(cd $(dirname $0) ; pwd)
source $CWD/common.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="linux64.incr"
export CHPL_SYSTEM_PREDIFF=$CHPL_HOME/util/test/prediff-for-incremental-warning

$CWD/nightly -cron -examples -no-llvm -compopts --incremental
