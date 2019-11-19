#!/usr/bin/env bash
#
# Test comm=ofi on linux64.

CWD=$(cd $(dirname $0) ; pwd)

source $CWD/common-ofi.bash || \
  ( echo "Could not set up comm=ofi testing." && exit 1 )

source $CWD/common-localnode-paratest.bash

export CHPL_NIGHTLY_TEST_DIRS="release/examples"

export CHPL_NIGHTLY_TEST_CONFIG_NAME="ofi"

$CWD/nightly -cron -futures $(get_nightly_paratest_args 3)
