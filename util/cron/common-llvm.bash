#!/usr/bin/env bash
#
# Setup environment for LLVM testing.

export CHPL_LLVM=llvm

# Run examples and test/extern/ferguson/.
export CHPL_START_TEST_ARGS="release/examples extern/ferguson"
