#!/usr/bin/env bash
#
# Test default configuration on full suite with baseline enabled on linux64.

CWD=$(cd $(dirname $0) ; pwd)
source $CWD/common.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="str-as-rec-baseline"

git branch -D string-as-rec
git checkout string-as-rec

$CWD/nightly -cron -baseline
