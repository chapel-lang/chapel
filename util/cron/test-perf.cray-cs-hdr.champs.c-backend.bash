#!/usr/bin/env bash
#
# Run CHAMPS testing on a cray-cs with HDR IB and C backend

UTIL_CRON_DIR=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)

export CHPL_TEST_PERF_CONFIG_NAME='16-node-cs-hdr'
export CHPL_NIGHTLY_TEST_CONFIG_NAME="perf.cray-cs-hdr.champs.c-backend"
export CHPL_TEST_PERF_DESCRIPTION='c-backend'

source $UTIL_CRON_DIR/common-champs.bash

# use clang for C backend testing, not GCC
# GCC consumes inordinate amounts of memory
export CHPL_TARGET_COMPILER=clang

export CHPL_TARGET_CPU=none

$UTIL_CRON_DIR/nightly -cron ${nightly_args}

sync_graphs
