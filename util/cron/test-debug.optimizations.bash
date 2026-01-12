#!/usr/bin/env bash
#
# Test --debug-symbols and --debug-safe-optimizations configuration on full suite.
# also enables --no-local to cover more test cases

UTIL_CRON_DIR=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)
source $UTIL_CRON_DIR/common.bash
source $UTIL_CRON_DIR/common-localnode-paratest.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="debug.optimizations"

$UTIL_CRON_DIR/nightly -cron -no-local -debug-optimizations -debug-symbols $(get_nightly_paratest_args)
