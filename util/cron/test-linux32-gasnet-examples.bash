#!/usr/bin/env bash
#
# Test default configuration with comm=gasnet against examples on 32bit linux.

CWD=$(cd $(dirname $0) ; pwd)
source $CWD/common-gasnet.bash

$CWD/nightly -cron -examples
