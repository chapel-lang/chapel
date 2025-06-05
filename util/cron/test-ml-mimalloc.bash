#!/usr/bin/env bash
#
# Test with mimalloc allocator, multilocale tests only

UTIL_CRON_DIR=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)
source $UTIL_CRON_DIR/common-gasnet.bash
source $UTIL_CRON_DIR/common-localnode-paratest.bash
unset CHPL_LAUNCHER

export GASNET_QUIET=Y

export CHPL_HOST_MEM=mimalloc
export CHPL_TARGET_MEM=mimalloc

export CHPL_NIGHTLY_TEST_CONFIG_NAME="ml-mimalloc"

$UTIL_CRON_DIR/nightly -cron -multilocale ${nightly_args} $(get_nightly_paratest_args 2)
