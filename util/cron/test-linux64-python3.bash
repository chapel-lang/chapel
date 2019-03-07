#!/usr/bin/env bash
#
# Test default configuration on examples only, on linux64, with python 3

CWD=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)
source $CWD/common.bash

# Python 3 env is setup by jenkins

export CHPL_NIGHTLY_TEST_CONFIG_NAME="linux64-python3"

$CWD/nightly -cron -examples ${nightly_args}
