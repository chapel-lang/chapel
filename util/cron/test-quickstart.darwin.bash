#!/usr/bin/env bash
#
# Test quickstart configuration on examples only, on darwin

UTIL_CRON_DIR=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)
source $UTIL_CRON_DIR/common.bash
source $UTIL_CRON_DIR/common-quickstart.bash
source $UTIL_CRON_DIR/common-darwin.bash
source $UTIL_CRON_DIR/common-localnode-paratest.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="quickstart.darwin"

$UTIL_CRON_DIR/nightly -cron -examples $(get_nightly_paratest_args)
