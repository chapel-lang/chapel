#!/usr/bin/env bash
#
# Test full suite for default configuration on darwin

UTIL_CRON_DIR=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)
source $UTIL_CRON_DIR/common.bash
source $UTIL_CRON_DIR/common-darwin.bash
source $UTIL_CRON_DIR/common-localnode-paratest.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="darwin"

$UTIL_CRON_DIR/nightly -cron $(get_nightly_paratest_args)
