#!/usr/bin/env bash
#
# Run CHAMPS testing on a cray-cs with HDR IB and LLVM backend

CWD=$(cd $(dirname $0) ; pwd)

export CHPL_TEST_PERF_CONFIG_NAME='16-node-cs-hdr'
export CHPL_NIGHTLY_TEST_CONFIG_NAME="perf.cray-cs-hdr.champs.llvm"
export CHPL_TEST_PERF_DESCRIPTION='llvm'

source $CWD/common-champs.bash

export CHPL_TARGET_CPU=none

$CWD/nightly -cron ${nightly_args} -no-buildcheck

sync_graphs
