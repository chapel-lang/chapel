#!/usr/bin/env bash
#
# Test default configuration with comm=gasnet against examples on 32bit linux.

CWD=$(cd $(dirname $0) ; pwd)
source $CWD/common-gasnet.bash
# put in for qthreads testing, because for 32 bit default 8M * 128
# preallocated causes segfaults
export CHPL_RT_CALL_STACK_SIZE=2M
$CWD/nightly -cron -examples
