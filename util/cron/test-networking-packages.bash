#!/usr/bin/env bash
#
# Test HDFS and Curl package modules

UTIL_CRON_DIR=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)
source $UTIL_CRON_DIR/common.bash
source $UTIL_CRON_DIR/common-quickstart.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="networking-packages"
export CHPL_NIGHTLY_TEST_DIRS="library/packages/Curl library/packages/URL"

export CHPL_LAUNCHER=none

$UTIL_CRON_DIR/nightly -cron ${nightly_args}
