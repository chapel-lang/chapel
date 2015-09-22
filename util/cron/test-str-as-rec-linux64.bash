#!/usr/bin/env bash
#
# Test default configuration on full suite with compiler performance enabled on
# linux64.

CWD=$(cd $(dirname $0) ; pwd)
source $CWD/common.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="str-as-rec-linux64"

git branch -D string-as-rec
git checkout string-as-rec

nightly_args="-compperformance (default)"
$CWD/nightly -cron -futures ${nightly_args}
