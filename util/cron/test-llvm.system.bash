#!/usr/bin/env bash
#
# Full test suite with CHPL_LLVM=system and pass --llvm flag to
# compiler on linux64.
#
# No paratest.

CWD=$(cd $(dirname $0) ; pwd)
source $CWD/common.bash
source $CWD/common-llvm.bash system

# common-llvm restricts us to extern/ferguson, but we want all the tests
unset CHPL_NIGHTLY_TEST_DIRS

export CHPL_NIGHTLY_TEST_CONFIG_NAME="llvm.system"

log_info START nightly -cron ${nightly_args}
$CWD/nightly -cron ${nightly_args}
log_info nightly EXIT status $?
