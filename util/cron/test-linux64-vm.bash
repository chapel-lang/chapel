#!/usr/bin/env bash
#
# Test llvm --fast configuration on full suite with compiler performance
# enabled on linux64 on an experimental VM

CWD=$(cd $(dirname $0) ; pwd)
source $CWD/common.bash
source $CWD/common-fast.bash
source $CWD/common-llvm.bash
unset CHPL_NIGHTLY_TEST_DIRS

export CHPL_NIGHTLY_TEST_CONFIG_NAME="linux64-vm"

# Don't spam the team for a config that's just testing out a new VM
export CHPL_TEST_NOMAIL=1
export CHPL_NIGHTLY_DEBUG_EMAIL=eronagha@cray.com

nightly_args="${nightly_args} -compperformance (llvm-fast)"
$CWD/nightly -cron ${nightly_args}
