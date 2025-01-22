#!/usr/bin/env bash
#
# Test default configuration running hellos, on linux64, with python 3.7

UTIL_CRON_DIR=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)
source $UTIL_CRON_DIR/common.bash
source $UTIL_CRON_DIR/common-python.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="linux64-python37"

set_and_check_python_version "3.7.17"

$UTIL_CRON_DIR/nightly -cron -pythonDep ${nightly_args}
