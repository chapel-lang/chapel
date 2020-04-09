#!/usr/bin/env bash
#
# Test default configuration against examples measuring memory leaks.

CWD=$(cd $(dirname $0) ; pwd)
source $CWD/common.bash
source $CWD/common-memleaks.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="memleaks.examples"

$CWD/nightly -cron -memleaks $(memleaks_log examples) -examples
save_memleaks_log examples
