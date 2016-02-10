#!/usr/bin/env bash
#
# Test default configuration on full suite with jemalloc on linux64.

CWD=$(cd $(dirname $0) ; pwd)
source $CWD/common.bash
source $CWD/common-jemalloc.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="jemalloc-linux64"

$CWD/nightly -cron
