#!/usr/bin/env bash
#
# Test gasnet (segment fast) against full suite on linux64.

CWD=$(cd $(dirname $0) ; pwd)
source $CWD/common-gasnet.bash
source $CWD/common-localnode-paratest.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="gasnet-fast"

export GASNET_QUIET=Y

# Test a GASNet compile using the fast segment
export CHPL_GASNET_SEGMENT=fast

$CWD/nightly -cron -multilocale $(get_nightly_paratest_args 6)
