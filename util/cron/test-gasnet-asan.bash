#!/usr/bin/env bash
#
# Test ASAN-compatible configuration with gasnet & ASAN on linux64, running
# multilocale tests only.
#

CWD=$(cd $(dirname $0) ; pwd)
source $CWD/common-gasnet.bash
source $CWD/common-asan.bash
source $CWD/common-localnode-paratest.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="gasnet-asan"

export GASNET_QUIET=Y

# "Should be able to detect most memory errors, since remote r/w are
# performed using active messages instead of RDMA".
export CHPL_COMM_SUBSTRATE=udp
export CHPL_GASNET_SEGMENT=everything

$CWD/nightly -cron ${nightly_args} $(get_nightly_paratest_args 6) -multilocale

