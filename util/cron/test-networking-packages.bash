#!/usr/bin/env bash
#
# Test HDFS and Curl package modules

CWD=$(cd $(dirname $0) ; pwd)
source $CWD/common.bash
source $CWD/common-quickstart.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="networking-packages"
export CHPL_NIGHTLY_TEST_DIRS="library/packages/Curl library/packages/HDFS"

$CWD/nightly -cron ${nightly_args}
