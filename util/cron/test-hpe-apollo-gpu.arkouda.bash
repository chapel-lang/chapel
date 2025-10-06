#!/usr/bin/env bash
#
# Run arkouda correctness testing on a hpe-apollo with CHPL_GPU=cpu

UTIL_CRON_DIR=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)

export ARKOUDA_SKIP_CHECK_DEPS=true
export CHPL_TEST_ARKOUDA_MULTI_DIM=true
export CHPL_INSTANTIATION_LIMIT=1024  # This is needed for compiling multidim arkouda. See #27069
export ARKOUDA_PARQUET_TEST_DATA_DIR=/nas/store/khandeka/parquet-testing/data  # Enable edge case parquet testing

export CHPL_TEST_ARKOUDA_PERF=false
export ARKOUDA_DEVELOPER=true

export CHPL_NIGHTLY_TEST_CONFIG_NAME="hpe-apollo-hdr-gpu.arkouda"

module list

source $UTIL_CRON_DIR/common.bash
source $UTIL_CRON_DIR/common-hpe-apollo.bash

# setup arkouda
source $UTIL_CRON_DIR/common-arkouda-hpe-apollo-hdr.bash
source $UTIL_CRON_DIR/common-arkouda.bash

export CHPL_COMM=none
export CHPL_TARGET_CPU=none

export CHPL_LLVM_GCC_PREFIX='none'


# GPU setup
export CHPL_LOCALE_MODEL=gpu
export CHPL_TEST_GPU=true
export CHPL_GPU=cpu

# List of Arkouda server modules we exempt from testing (that goal is to
# eventually have this be an empty list).
export CHPL_TEST_ARKOUDA_DISABLE_MODULES=In1dMsg:HDF5Msg:HDF5Msg_LEGACY:HDF5MultiDim
export CHPL_TEST_ARKOUDA_STOP_AFTER_BUILD="true"



nightly_args="${nightly_args} -no-buildcheck"

module list

test_nightly
