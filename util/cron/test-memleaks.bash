#!/usr/bin/env bash
#
# Test default configuration against full suite measuring memory leaks.

CWD=$(cd $(dirname $0) ; pwd)
source $CWD/common.bash
source $CWD/common-memleaks.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="memleaks"

$CWD/nightly -cron -memleaks $(memleaks_log full) -no-futures
save_memleaks_log full
