#!/usr/bin/env bash
#
# Run issue tracker on chapcs

CWD=$(cd $(dirname $0) ; pwd)

export CHPL_TEST_PERF_CONFIG_NAME='chapcs'
export CHPL_NIGHTLY_TEST_CONFIG_NAME="perf.chapcs.issuetracker"
source $CWD/common-issuetracker.bash

test_nightly
sync_graphs
