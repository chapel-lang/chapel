#!/usr/bin/env bash
#
# Full test suite with CHPL_LLVM=none and CHPL_TARGET_COMPILER=gnu
# on linux64. Now uses paratest.server.

UTIL_CRON_DIR=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)
source $UTIL_CRON_DIR/common.bash
source $UTIL_CRON_DIR/common-c-backend.bash
source $UTIL_CRON_DIR/common-localnode-paratest.bash

# common-llvm restricts us to extern/ferguson, but we want all the tests
unset CHPL_NIGHTLY_TEST_DIRS

nightly_args="${nightly_args} $(set +x ; get_nightly_paratest_args) -asserts"

export CHPL_NIGHTLY_TEST_CONFIG_NAME="c-backend"

log_info START nightly -cron ${nightly_args}
$UTIL_CRON_DIR/nightly -cron ${nightly_args}
log_info nightly EXIT status $?
