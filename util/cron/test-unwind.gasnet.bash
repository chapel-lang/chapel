#!/usr/bin/env bash
#
# Test with bundled libunwind

UTIL_CRON_DIR=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)

source $UTIL_CRON_DIR/common-gasnet.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="unwind.gasnet"
export CHPL_NIGHTLY_TEST_DIRS="runtime/stacktrace"

export CHPL_UNWIND=bundled

export GASNET_QUIET=y  # otherwise we get warnings that do match the .good

$UTIL_CRON_DIR/nightly -cron
