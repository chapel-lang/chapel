#!/usr/bin/env bash
#
# Test quickstart configuration on full suite on linux64.

UTIL_CRON_DIR=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)
source $UTIL_CRON_DIR/common.bash
source $UTIL_CRON_DIR/common-quickstart.bash
source $UTIL_CRON_DIR/common-localnode-paratest.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="quickstart"

$UTIL_CRON_DIR/nightly -cron -chpl-language-server $(get_nightly_paratest_args 8)
