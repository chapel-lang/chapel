#!/usr/bin/env bash
#
# Run CHAMPS testing on a cray-cs with HDR IB and C backend

CWD=$(cd $(dirname $0) ; pwd)

export CHPL_TEST_PERF_CONFIG_NAME='16-node-cs-hdr'
export CHPL_NIGHTLY_TEST_CONFIG_NAME="perf.cray-cs-hdr.champs.c-backend"
export CHPL_TEST_PERF_DESCRIPTION='c-backend'

source $CWD/common-champs.bash
source $CWD/common-c-backend.bash

$CWD/nightly -cron ${nightly_args} -no-buildcheck

sync_graphs
