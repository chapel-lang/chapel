#!/usr/bin/env bash

UTIL_CRON_DIR=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)

source $UTIL_CRON_DIR/common.bash
source $UTIL_CRON_DIR/common-ofi.bash || \
  ( echo "Could not set up comm=ofi testing." && exit 1 )
source $UTIL_CRON_DIR/common-hpe-cray-ex.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="hpe-cray-ex-ofi"
export CHPL_RT_COMM_OFI_EXPECTED_PROVIDER="cxi"
export CHPL_RT_MAX_HEAP_SIZE=16g
export SLURM_PARTITION=allnodes 


# test a small subset of all tests due to limited resources
# ideally, we should run the whole suite
export CHPL_NIGHTLY_TEST_DIRS="release/examples/ runtime/configMatters/ multilocale/"

$UTIL_CRON_DIR/nightly -cron -blog ${nightly_args}
