#!/usr/bin/env bash
#
# Test --verify configuration on full suite on linux64.

CWD=$(cd $(dirname $0) ; pwd)
source $CWD/common.bash
$CWD/nightly -cron -no-futures -verify
