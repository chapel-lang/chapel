#!/usr/bin/env bash
#
# Setup environment for LLVM testing.

CWD=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)

export CHPL_LLVM=llvm

# Run examples and test/extern/ferguson/.
export CHPL_START_TEST_ARGS="release/examples extern/ferguson"

suppression_file=$CWD/../../test/Suppressions/llvm.suppress
nightly_args="-llvm -suppress ${suppression_file}"
