#!/usr/bin/env bash
#
# Test --no-local configuration on full suite.

UTIL_CRON_DIR=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)

source $UTIL_CRON_DIR/common.bash
source $UTIL_CRON_DIR/common-localnode-paratest.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="no-local"

nightly_args="${nightly_args} -no-local"
$UTIL_CRON_DIR/nightly -cron ${nightly_args} $(get_nightly_paratest_args 8)
