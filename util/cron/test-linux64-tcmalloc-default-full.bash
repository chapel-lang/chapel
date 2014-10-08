#!/usr/bin/env bash
#
# Test tcmalloc mem with default configuration against full suite.

CWD=$(cd $(dirname $0) ; pwd)
source $CWD/common.bash
source $CWD/common-tcmalloc.bash

$CWD/nightly -cron -no-futures
