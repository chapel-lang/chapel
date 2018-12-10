#!/usr/bin/env bash
#
# Run tests that use generated c2chapel files

CWD=$(cd $(dirname $0) ; pwd)
source $CWD/common.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="c2chapel"

export CHPL_NIGHTLY_TEST_DIRS="c2chapel/"

# test/c2chapel/SKIPIF relies on 'c2chapel' being in $PATH.
#
# Depending on how this script is called, $PATH may have been reset (e.g. a
# .bashrc was sourced).
#
# TODO: emit useful error if tests will be skipped
CHPL_BIN_SUBDIR=`"$CHPL_HOME"/util/chplenv/chpl_bin_subdir.py`
export PATH="$CHPL_HOME/bin/$CHPL_BIN_SUBDIR:$PATH"

$CWD/nightly -cron
