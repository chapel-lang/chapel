#!/usr/bin/env bash
#
# Test default configuration against examples on RHEL linux64 system.

UTIL_CRON_DIR=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)
source $UTIL_CRON_DIR/common.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="rhel.linux64"

# limit the amount of parallelism used, we were running out of memory on the
# testing machine.
export CHPL_MAKE_MAX_CPU_COUNT=2

$UTIL_CRON_DIR/nightly -cron -examples
