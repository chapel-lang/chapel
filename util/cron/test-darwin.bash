#!/usr/bin/env bash
#
# Test default configuration on darwin

CWD=$(cd $(dirname $0) ; pwd)
source $CWD/common.bash
source $CWD/common-darwin.bash
source $CWD/common-localnode-paratest.bash

# TODO: To be promoted to common.bash if this works
# Work-around to remove git submodules
#   See Cray/chapel-private#1050 for long term solution
git clean -ffdx ${CHPL_HOME}/test/mason

export CHPL_NIGHTLY_TEST_CONFIG_NAME="darwin"

$CWD/nightly -cron $(get_nightly_paratest_args)
