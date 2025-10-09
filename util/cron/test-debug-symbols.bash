#!/usr/bin/env bash
#
# Test --fast configuration on full suite.

UTIL_CRON_DIR=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)
source $UTIL_CRON_DIR/common.bash
source $UTIL_CRON_DIR/common-localnode-paratest.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="debug-symbols"

$UTIL_CRON_DIR/nightly -cron -debug-symbols $(get_nightly_paratest_args)
