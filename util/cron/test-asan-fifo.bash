#!/usr/bin/env bash
#
# Test ASAN-compatible configuration on full suite with ASAN on linux64.

UTIL_CRON_DIR=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)
source $UTIL_CRON_DIR/common.bash
source $UTIL_CRON_DIR/common-asan.bash
source $UTIL_CRON_DIR/common-localnode-paratest.bash

export CHPL_TASKS=fifo
# don't asan the compiler
export CHPL_SANITIZE=none
export CHPL_SANITIZE_EXE=address

export CHPL_NIGHTLY_TEST_CONFIG_NAME="asan-fifo"

$UTIL_CRON_DIR/nightly -cron ${nightly_args} $(get_nightly_paratest_args 8)
