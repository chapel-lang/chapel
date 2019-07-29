#!/bin/bash
#
# Test generated Python modules

CWD=$(cd $(dirname $0) ; pwd)
source $CWD/common-gasnet.bash

export CHPL_LIB_PIC=pic

export CHPL_NIGHTLY_TEST_CONFIG_NAME="python-modules.gasnet"
export CHPL_NIGHTLY_TEST_DIRS="interop/python/multilocale"

$CWD/nightly -cron -futures
