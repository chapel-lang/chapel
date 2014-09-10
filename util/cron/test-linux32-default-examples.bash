#!/usr/bin/env bash
#
# Test default configuration against examples on 32bit linux.

CWD=$(cd $(dirname $0) ; pwd)
source $CWD/common.bash

$CWD/nightly -cron -examples
