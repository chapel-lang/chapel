#!/usr/bin/env bash
#
# Test tcmalloc mem with comm=gasnet against full suite.

CWD=$(cd $(dirname $0) ; pwd)
source $CWD/common.bash
$CWD/nightly.tcmalloc-gasnet
