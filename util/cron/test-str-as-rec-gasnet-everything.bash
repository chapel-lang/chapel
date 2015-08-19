#!/usr/bin/env bash
#
# Test gasnet (segment everything) against full suite on linux64.

CWD=$(cd $(dirname $0) ; pwd)
source $CWD/common-gasnet.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="str-as-rec-gasnet-everything"

export GASNET_QUIET=Y

# Test a GASNet compile using the default segment (everything for linux64)
export CHPL_GASNET_SEGMENT=everything

git checkout string-as-rec

$CWD/nightly -cron -futures
