#!/usr/bin/env bash
#
# Test default configuration on chplcheck and chpl-language-server tests only

UTIL_CRON_DIR=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)

source $UTIL_CRON_DIR/common.bash
source $UTIL_CRON_DIR/common-localnode-paratest.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="linux64-chplcheck-cls-only"

$UTIL_CRON_DIR/nightly -cron -asserts -chplcheckonly -chpl-language-server ${nightly_args} $(get_nightly_paratest_args 8)
