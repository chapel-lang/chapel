#!/usr/bin/env bash
#
# Test gasnet smp against hellos on linux64.

UTIL_CRON_DIR=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)
source $UTIL_CRON_DIR/common.bash
source $UTIL_CRON_DIR/common-localnode-paratest.bash

# setup for smp
export CHPL_COMM=gasnet
export CHPL_COMM_SUBSTRATE=smp

export CHPL_NIGHTLY_TEST_CONFIG_NAME="gasnet-smp"

$UTIL_CRON_DIR/nightly -cron -multilocale $(get_nightly_paratest_args 8)
