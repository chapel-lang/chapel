#!/usr/bin/env bash
#
# Test hellos with chpl built with jemalloc allocator

CWD=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)
source $CWD/common.bash

export CHPL_HOST_MEM="jemalloc"
export CHPL_NIGHTLY_TEST_CONFIG_NAME="host-jemalloc"

$CWD/nightly -cron -hellos ${nightly_args}
