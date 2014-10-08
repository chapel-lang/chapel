#!/usr/bin/env bash
#
# Test numa locale model with default configuration on full suite.

CWD=$(cd $(dirname $0) ; pwd)
source $CWD/common.bash
source $CWD/common-numa.bash

$CWD/nightly -cron
