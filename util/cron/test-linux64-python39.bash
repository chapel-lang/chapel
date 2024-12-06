#!/usr/bin/env bash
#
# Test default configuration running hellos, on linux64, with python 3.9

UTIL_CRON_DIR=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)
source $UTIL_CRON_DIR/common.bash
source $UTIL_CRON_DIR/common-python.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="linux64-python39"

set_python_version "3.9"

# Check correct Python version loaded
expected_python_version="3.9.13"
actual_python_version=$(python3 --version | cut -d' ' -f2)
if [ "$actual_python_version" != "$expected_python_version" ]; then
  echo "Wrong Python version"
  echo "Expected Version: $expected_python_version. Actual Version: $actual_python_version"
  exit 2
fi

$UTIL_CRON_DIR/nightly -cron -hellos ${nightly_args}
