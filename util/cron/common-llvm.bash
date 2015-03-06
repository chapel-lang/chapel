#!/usr/bin/env bash
#
# Setup environment for LLVM testing.

CWD=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)

export CHPL_LLVM=llvm

# Run examples and test/extern/ferguson/.
export CHPL_NIGHTLY_TEST_DIRS="extern/ferguson"

nightly_args="-llvm"
