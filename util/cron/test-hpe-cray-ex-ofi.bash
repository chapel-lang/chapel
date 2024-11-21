#!/usr/bin/env bash

CWD=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)

source $CWD/common.bash
source $CWD/common-ofi.bash || \
  ( echo "Could not set up comm=ofi testing." && exit 1 )
source $CWD/common-hpe-cray-ex.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="hpe-cray-ex-ofi"
export CHPL_RT_COMM_OFI_EXPECTED_PROVIDER="cxi"
export CHPL_RT_MAX_HEAP_SIZE=16g

$CWD/nightly -cron -examples -blog ${nightly_args}
