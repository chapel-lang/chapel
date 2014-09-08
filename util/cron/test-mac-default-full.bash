#!/usr/bin/env bash
#
# Test default configuration against examples on mac os x.

CWD=$(cd $(dirname $0) ; pwd)
source $CWD/common.bash

$CWD/nightly -cron
