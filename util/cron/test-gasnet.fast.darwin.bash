#!/usr/bin/env bash
#
# Test gasnet (segment fast) against hellos on darwin

UTIL_CRON_DIR=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)
source $UTIL_CRON_DIR/common-gasnet.bash
export CHPL_GASNET_SEGMENT=fast
source $UTIL_CRON_DIR/common-darwin.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="gasnet.fast.darwin"

$UTIL_CRON_DIR/nightly -cron -hellos
