#!/usr/bin/env bash
#
# Test compiler performance of release/examples with default configuration

CWD=$(cd $(dirname $0) ; pwd)

export CHPL_TEST_PERF_CONFIG_NAME='comp-default'

source $CWD/common.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="linux64-comp"

nightly_args="${nightly_args} -examples -componly -compperformance (default)"
$CWD/nightly -cron  ${nightly_args}
