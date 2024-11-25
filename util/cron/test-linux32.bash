#!/usr/bin/env bash
#
# Test default configuration against full suite on 32bit linux.

UTIL_CRON_DIR=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)
source $UTIL_CRON_DIR/common.bash
source $UTIL_CRON_DIR/common-localnode-paratest.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="linux32"

$UTIL_CRON_DIR/nightly -cron $(get_nightly_paratest_args)
