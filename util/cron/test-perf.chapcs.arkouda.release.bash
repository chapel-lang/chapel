#!/usr/bin/env bash
#
# Run arkouda testing on chapcs

UTIL_CRON_DIR=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)

export CHPL_TEST_PERF_CONFIG_NAME='chapcs'
export CHPL_NIGHTLY_TEST_CONFIG_NAME="perf.chapcs.arkouda.release"
source $UTIL_CRON_DIR/common-arkouda.bash

setup_release false

nightly_args="${nightly_args} -no-warnings"

export CHPL_TEST_PERF_DESCRIPTION=release
export CHPL_TEST_PERF_CONFIGS="release:v,nightly:v"

test_release
sync_graphs
