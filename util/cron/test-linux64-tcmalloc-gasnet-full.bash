#!/usr/bin/env bash
#
# Test tcmalloc mem with comm=gasnet against full suite.

CWD=$(cd $(dirname $0) ; pwd)
source $CWD/common-gasnet.bash
source $CWD/common-tcmalloc.bash

export GASNET_QUIET=Y

$CWD/nightly -cron -multilocale -no-futures
