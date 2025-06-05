#!/usr/bin/env bash
#
# Test default configuration against examples measuring memory leaks.

UTIL_CRON_DIR=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)
source $UTIL_CRON_DIR/common.bash
source $UTIL_CRON_DIR/common-memleaks.bash

export CHPL_LAUNCHER=none

export CHPL_NIGHTLY_TEST_CONFIG_NAME="memleaks.examples"

$UTIL_CRON_DIR/nightly -cron -memleaks -examples
