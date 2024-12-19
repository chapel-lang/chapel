#!/usr/bin/env bash
#
# Test gasnet (segment everything) against multilocale tests on darwin

UTIL_CRON_DIR=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)
source $UTIL_CRON_DIR/common-gasnet.bash
source $UTIL_CRON_DIR/common-darwin.bash
source $UTIL_CRON_DIR/common-localnode-paratest.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="gasnet.darwin"

$UTIL_CRON_DIR/nightly -cron -multilocale $(get_nightly_paratest_args)
