#!/usr/bin/env bash
#
# Run arkouda testing on chapcs

CWD=$(cd $(dirname $0) ; pwd)

export CHPL_TEST_PERF_CONFIG_NAME='chapcs'
export CHPL_NIGHTLY_TEST_CONFIG_NAME="perf.chapcs.arkouda.release"
source $CWD/common-arkouda.bash

test_release
sync_graphs
