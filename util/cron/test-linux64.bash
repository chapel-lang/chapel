#!/usr/bin/env bash
#
# Test default configuration on full suite.

CWD=$(cd $(dirname $0) ; pwd)

source $CWD/common.bash
source $CWD/common-localnode-paratest.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="linux64"

$CWD/nightly -cron -mason -futures ${nightly_args} $(get_nightly_paratest_args)
