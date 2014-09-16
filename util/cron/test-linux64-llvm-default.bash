#!/usr/bin/env bash
#
# Test default configuration with CHPL_LLVM=llvm and pass --llvm flag to
# compiler on linux64.

CWD=$(cd $(dirname $0) ; pwd)
source $CWD/common.bash

export CHPL_LLVM=llvm

# Run hellos and test/extern/ferguson/.
export CHPL_START_TEST_ARGS="release/examples/hello*.chpl extern/ferguson"

$CWD/nightly -cron -llvm
