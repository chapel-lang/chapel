#!/usr/bin/env bash
#
# Test compiler performance of release/examples with --fast

CWD=$(cd $(dirname $0) ; pwd)

export CHPL_TEST_PERF_CONFIG_NAME='comp-fast'

source $CWD/common.bash
source $CWD/common-fast.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="fast-comp"

nightly_args="${nightly_args} -examples -componly -compperformance (--fast)"
$CWD/nightly -cron ${nightly_args}
