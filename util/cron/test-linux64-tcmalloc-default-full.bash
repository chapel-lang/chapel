#!/usr/bin/env bash
#
# Test tcmalloc mem with default configuration against full suite.

CWD=$(cd $(dirname $0) ; pwd)
source $CWD/common.bash
$CWD/nightly.tcmalloc
