#!/usr/bin/env bash
#
# Test default configuration against full suite measuring memory leaks.

UTIL_CRON_DIR=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)
source $UTIL_CRON_DIR/common.bash
source $UTIL_CRON_DIR/common-memleaks.bash
source $UTIL_CRON_DIR/common-localnode-paratest.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="memleaks"

$UTIL_CRON_DIR/nightly -cron -memleaks -memleakslog $(memleaks_log full) -no-futures $(get_nightly_paratest_args 8)
save_memleaks_log full
