#!/usr/bin/env bash
#
# Test valgrind-compatible configuration on full suite with valgrind on linux64.

CWD=$(cd $(dirname $0) ; pwd)
source $CWD/common.bash
source $CWD/common-valgrind.bash
source $CWD/common-localnode-paratest.bash

# valgrind serializes execution, so don't limit to one executable at a time
unset CHPL_TEST_LIMIT_RUNNING_EXECUTABLES

export CHPL_NIGHTLY_TEST_CONFIG_NAME="valgrind"

$CWD/nightly -cron ${nightly_args} $(get_nightly_paratest_args 20)
