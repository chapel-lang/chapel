#!/usr/bin/env bash
#
# Test default configuration on full suite with valgrind on linux64.

CWD=$(cd $(dirname $0) ; pwd)
source $CWD/common.bash
source $CWD/common-fifo.bash
source $CWD/common-valgrind.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="valgrind"

$CWD/nightly -cron ${nightly_args}
