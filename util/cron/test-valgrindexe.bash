#!/usr/bin/env bash
#
# Test valgrind-compatible configuration on full suite with valgrind on linux64.

UTIL_CRON_DIR=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)
source $UTIL_CRON_DIR/common.bash
source $UTIL_CRON_DIR/common-valgrind.bash
source $UTIL_CRON_DIR/common-localnode-paratest.bash

# valgrind serializes execution, so don't limit to one executable at a time
unset CHPL_TEST_LIMIT_RUNNING_EXECUTABLES

export CHPL_NIGHTLY_TEST_CONFIG_NAME="valgrindexe"

$UTIL_CRON_DIR/nightly -cron -valgrindexe ${nightly_args} $(get_nightly_paratest_args 8)
