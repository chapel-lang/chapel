#!/usr/bin/env bash
#
# Test ASAN-compatible configuration with gasnet & ASAN on linux64, running
# multilocale tests only.
#

CWD=$(cd $(dirname $0) ; pwd)
source $CWD/common-gasnet.bash
source $CWD/common-asan.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="gasnet-asan.arkouda"

export GASNET_QUIET=Y

# Cache does not work with ASan, quiet warnings about it for testing
export CHPL_RT_CACHE_QUIET=true

# The ASAN best practices doc recommends that SUBSTRATE be set to 'udp' and
# SEGMENT be set to 'everything' in order to encourage active messages
# instead of RDMA. This will let ASAN detect more memory errors.
#
export CHPL_COMM_SUBSTRATE=udp
export CHPL_GASNET_SEGMENT=everything

# Use Arrow dependencies built using asan
export ARKOUDA_ASAN=true

export CHPL_TEST_ARKOUDA_PERF=false

source $CWD/common-arkouda.bash

export CHPL_FLAGS="--parallel-make 16 --ccflags -Og"

test_correctness
