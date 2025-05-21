#!/usr/bin/env bash
#
# Run arkouda correctness testing on a hpe-apollo with HDR IB

UTIL_CRON_DIR=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)

export ARKOUDA_DEP_DIR=/hpelustre/chapelu/arkouda-deps
export ARKOUDA_SKIP_CHECK_DEPS=true
export CHPL_TEST_ARKOUDA_MULTI_DIM=true
export CHPL_INSTANTIATION_LIMIT=1024 # This is needed for compiling multidim arkouda. See #27069

export CHPL_TEST_ARKOUDA_PERF=false
export ARKOUDA_DEVELOPER=true

export CHPL_NIGHTLY_TEST_CONFIG_NAME="hpe-apollo-hdr.arkouda.release"

module list

source $UTIL_CRON_DIR/common.bash
source $UTIL_CRON_DIR/common-hpe-apollo.bash
#
# setup arkouda
source $UTIL_CRON_DIR/common-arkouda.bash
source $UTIL_CRON_DIR/common-arkouda-hpe-apollo-hdr.bash

export ARKOUDA_NUMLOCALES=16

export CHPL_GASNET_SEGMENT=fast
export GASNET_PHYSMEM_MAX="0.90"

export CHPL_LLVM_GCC_PREFIX='none'

if [ -n "$CHPL_WHICH_RELEASE_FOR_ARKOUDA" ]; then
  release_dependencies
else
  echo "CHPL_WHICH_RELEASE_FOR_ARKOUDA not set, cannot run Arkouda release test!"
  exit 1
fi


nightly_args="${nightly_args} -no-buildcheck"

module list

test_release
