#!/usr/bin/env bash
#
# Test default configuration on full suite with valgrind on linux64.

CWD=$(cd $(dirname $0) ; pwd)
source $CWD/common.bash
source $CWD/common-fifo.bash
source $CWD/common-valgrind.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="valgrind"

# EJR (02/22/16): use cstdlib until the valgrind header files are installed on
# our test systems (header files required for jemalloc to support valgrind.)
export CHPL_MEM=cstdlib

$CWD/nightly -cron ${nightly_args}
