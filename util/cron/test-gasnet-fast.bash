#!/usr/bin/env bash
#
# Test gasnet (segment fast) against full suite on linux64.

UTIL_CRON_DIR=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)
source $UTIL_CRON_DIR/common-gasnet.bash
source $UTIL_CRON_DIR/common-localnode-paratest.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="gasnet-fast"

export GASNET_QUIET=Y

# Test a GASNet compile using the fast segment
export CHPL_GASNET_SEGMENT=fast
unset CHPL_LAUNCHER

$UTIL_CRON_DIR/nightly -cron -multilocale $(get_nightly_paratest_args 8)
