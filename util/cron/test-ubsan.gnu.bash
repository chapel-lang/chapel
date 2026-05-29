#!/usr/bin/env bash
#
# Test UBSAN-compatible configuration on full suite with UBSAN on linux64.

UTIL_CRON_DIR=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)
source $UTIL_CRON_DIR/common.bash
source $UTIL_CRON_DIR/common-ubsan.bash
export CHPL_TARGET_COMPILER=gnu
source $UTIL_CRON_DIR/common-localnode-paratest.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="ubsan.gnu"

$UTIL_CRON_DIR/nightly -cron ${nightly_args} $(get_nightly_paratest_args 16)
