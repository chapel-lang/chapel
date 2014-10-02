#!/usr/bin/env bash
#
# Test default configuration on full suite with compiler performance enabled on
# linux64.

CWD=$(cd $(dirname $0) ; pwd)
source $CWD/common.bash

nightly_args="-compperformance (default)"
$CWD/nightly -cron ${nightly_args}
