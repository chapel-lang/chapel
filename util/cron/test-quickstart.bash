#!/usr/bin/env bash
#
# Test quickstart configuration on full suite on linux64.

CWD=$(cd $(dirname $0) ; pwd)
source $CWD/common.bash
source $CWD/common-quickstart.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="quickstart"

$CWD/nightly -cron
