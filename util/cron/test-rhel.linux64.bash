#!/usr/bin/env bash
#
# Test default configuration against examples on RHEL linux64 system.

UTIL_CRON_DIR=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)
source $UTIL_CRON_DIR/common.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="rhel.linux64"

$UTIL_CRON_DIR/nightly -cron -examples
