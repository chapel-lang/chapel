#!/usr/bin/env bash
#
# Test compiler performance of release/examples with --no-local

CWD=$(cd $(dirname $0) ; pwd)

export CHPL_TEST_PERF_CONFIG_NAME='comp-no-local'

source $CWD/common.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="no-local-comp"

nightly_args="${nightly_args} -no-local -examples -componly -compperformance (--no-local)"
$CWD/nightly -cron ${nightly_args}
