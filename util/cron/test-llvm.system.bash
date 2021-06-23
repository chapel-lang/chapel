#!/usr/bin/env bash
#
# Full test suite with CHPL_LLVM=system and CHPL_TARGET_COMPILER=llvm
# on linux64.
#
# No paratest.

CWD=$(cd $(dirname $0) ; pwd)

# use the official system installed LLVM instead of the versions in
# /data/cf/chapel/llvm or /cray/css/users/chapelu/llvm
export OFFICIAL_SYSTEM_LLVM=T

source $CWD/common.bash
source $CWD/common-llvm.bash system
source $CWD/common-localnode-paratest.bash

unset OFFICIAL_SYSTEM_LLVM

# common-llvm restricts us to extern/ferguson, but we want all the tests
unset CHPL_NIGHTLY_TEST_DIRS

export CHPL_NIGHTLY_TEST_CONFIG_NAME="llvm.system"

log_info START nightly -cron ${nightly_args}
$CWD/nightly -cron ${nightly_args} $(get_nightly_paratest_args)
log_info nightly EXIT status $?
