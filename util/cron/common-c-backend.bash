#!/usr/bin/env bash
#
# Configure environment for testing C backends

UTIL_CRON_DIR=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)

# Do not set CHPL_LLVM=none here for all c-backend testing.
# Generally speaking, we would like for tests of the C backend
# to build with LLVM so that we can test extern blocks with C
# code generation.
# Specific configs will set CHPL_LLVM=none (e.g. quickstart testing).

export CHPL_TARGET_COMPILER=gnu
