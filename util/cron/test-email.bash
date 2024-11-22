#!/usr/bin/env bash
#
# Test hellos with chpl built with jemalloc allocator

UTIL_CRON_DIR=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)
source $UTIL_CRON_DIR/common.bash

export CHPL_HOST_MEM="jemalloc"
export CHPL_NIGHTLY_TEST_CONFIG_NAME="test-email"

$UTIL_CRON_DIR/nightly -cron -hellos ${nightly_args}
