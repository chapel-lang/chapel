#!/usr/bin/env bash
#
# Test default configuration against examples measuring memory leaks.

CWD=$(cd $(dirname $0) ; pwd)
source $CWD/common.bash
$CWD/nightly.memleaks -examples
