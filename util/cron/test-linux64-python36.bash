#!/usr/bin/env bash
#
# Test default configuration running hellos, on linux64, with python 3.6

UTIL_CRON_DIR=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)
source $UTIL_CRON_DIR/common.bash
source $UTIL_CRON_DIR/common-python.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="linux64-python36"

set_python_version "3.6"

# Check correct Python version loaded
expected_python_version="3.6.10"
actual_python_version=$(python3 --version | cut -d' ' -f2)
if [ "$actual_python_version" != "$expected_python_version" ]; then
  echo "Wrong Python version"
  echo "Expected Version: $expected_python_version. Actual Version: $actual_python_version"
  exit 2
fi

$UTIL_CRON_DIR/nightly -cron -pythonDep ${nightly_args}
