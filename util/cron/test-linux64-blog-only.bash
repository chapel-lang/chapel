#!/usr/bin/env bash
#
# Test default configuration on blog posts only

UTIL_CRON_DIR=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)

source $UTIL_CRON_DIR/common.bash
source $UTIL_CRON_DIR/common-localnode-paratest.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="linux64-blog-only"

$UTIL_CRON_DIR/nightly -cron -blogonly ${nightly_args} $(get_nightly_paratest_args 8)
