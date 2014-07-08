#!/usr/bin/env bash
#
# Test numa locale model with comm=gasnet on multilocale and examples.

CWD=$(cd $(dirname $0) ; pwd)
source $CWD/common-gasnet.bash

# TODO: Do we need/want this? (thomasvandoren, 2014-07-01)
export GASNET_QUIET=Y

$CWD/nightly.numa -multilocale
