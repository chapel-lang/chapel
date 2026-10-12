#!/usr/bin/env bash
#
# Test with mimalloc allocator

UTIL_CRON_DIR=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)
source $UTIL_CRON_DIR/common.bash
source $UTIL_CRON_DIR/common-localnode-paratest.bash

export CHPL_HOST_MEM=mimalloc
export CHPL_TARGET_MEM=mimalloc

export CHPL_NIGHTLY_TEST_CONFIG_NAME="mimalloc"

$UTIL_CRON_DIR/nightly -cron ${nightly_args} $(get_nightly_paratest_args 8)
