#!/usr/bin/env bash
#
# Test --no-local configuration on full suite with compiler performance enabled
# on linux64.

UTIL_CRON_DIR=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)
source $UTIL_CRON_DIR/common.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="no-local.linux32"

$UTIL_CRON_DIR/nightly -cron -examples -no-local
