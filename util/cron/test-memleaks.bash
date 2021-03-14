#!/usr/bin/env bash
#
# Test default configuration against full suite measuring memory leaks.

CWD=$(cd $(dirname $0) ; pwd)
source $CWD/common.bash
source $CWD/common-memleaks.bash
source $CWD/common-localnode-paratest.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="memleaks"

$CWD/nightly -cron -memleakslog $(memleaks_log full) -no-futures $(get_nightly_paratest_args)
save_memleaks_log full
