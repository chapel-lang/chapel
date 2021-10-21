#!/usr/bin/env bash
#
# Test with bundled libunwind

CWD=$(cd $(dirname $0) ; pwd)

source $CWD/common-gasnet.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="unwind.gasnet"
export CHPL_NIGHTLY_TEST_DIRS="runtime/stacktrace"

export CHPL_UNWIND=bundled

export GASNET_QUIET=y  # otherwise we get warnings that do match the .good

$CWD/nightly -cron
