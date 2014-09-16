#!/usr/bin/env bash
#
# Setup environment for LLVM testing.

export CHPL_LLVM=llvm

# Run hellos and test/extern/ferguson/.
export CHPL_START_TEST_ARGS="release/examples/hello*.chpl extern/ferguson"

