#!/usr/bin/env bash
#
# Test cyclic distribution using default configuration.

UTIL_CRON_DIR=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)
source $UTIL_CRON_DIR/common-gasnet.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="dist-cyclic"

$UTIL_CRON_DIR/nightly -cron -dist cyclic
