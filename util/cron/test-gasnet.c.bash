#!/usr/bin/env bash
#
# Test gasnet configuration with CHPL_LLVM=none & CHPL_TARGET_COMPILER=gnu
# on linux64.

CWD=$(cd $(dirname $0) ; pwd)
source $CWD/common-gasnet.bash
source $CWD/common-c.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="gasnet.c"

# common-llvm sets this to a single test directory, but we want a wider test
# distribution for this configuration.
unset CHPL_NIGHTLY_TEST_DIRS

$CWD/nightly -cron -multilocale ${nightly_args}
