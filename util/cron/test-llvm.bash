#!/usr/bin/env bash
#
# Full test suite with CHPL_LLVM=llvm and pass --llvm flag to
# compiler on linux64. Now uses paratest.server.

# Needs /data/cf/chapel/setup_python27.bash (common-llvm)

CWD=$(cd $(dirname $0) ; pwd)
source $CWD/common.bash
source $CWD/common-llvm.bash
source $CWD/common-localnode-paratest.bash

# common-llvm restricts us to extern/ferguson, but we want all the tests
unset CHPL_NIGHTLY_TEST_DIRS

nightly_args="${nightly_args} $(set +x ; get_nightly_paratest_args) -asserts"

export CHPL_NIGHTLY_TEST_CONFIG_NAME="llvm"

log_info START nightly -cron ${nightly_args}
$CWD/nightly -cron ${nightly_args}
log_info nightly EXIT status $?
