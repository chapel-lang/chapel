#!/usr/bin/env bash
#
# Test gasnet smp against hellos on linux64.

CWD=$(cd $(dirname $0) ; pwd)
source $CWD/common.bash

# setup for smp
export CHPL_COMM=gasnet
export CHPL_COMM_SUBSTRATE=smp

export CHPL_NIGHTLY_TEST_CONFIG_NAME="gasnet-smp"

$CWD/nightly -cron -hellos ${nightly_args}
