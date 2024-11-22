#!/usr/bin/env bash
#
# Test incremental compilation on examples only,
# using default linux64 configuration.

UTIL_CRON_DIR=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)
source $UTIL_CRON_DIR/common.bash

export CHPL_TARGET_COMPILER=gnu
export CHPL_NIGHTLY_TEST_CONFIG_NAME="linux64.incr"
export CHPL_SYSTEM_PREDIFF=$CHPL_HOME/util/test/prediff-for-incremental-warning

$UTIL_CRON_DIR/nightly -cron -examples -compopts --incremental
