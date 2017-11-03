#!/usr/bin/env bash
#
# Test default configuration on full suite with compiler performance enabled on
# linux64 on an experimental VM

CWD=$(cd $(dirname $0) ; pwd)
source $CWD/common.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="linux64-vm"

# Don't spam the team for a config that's just testing out a new VM
export CHPL_TEST_NOMAIL=1
export CHPL_NIGHTLY_DEBUG_EMAIL=eronagha@cray.com

nightly_args="-compperformance (default)"
$CWD/nightly -cron -futures ${nightly_args}
