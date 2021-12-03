#!/usr/bin/env bash
#
# Test we can successfully build chplvis

CWD=$(cd $(dirname $0) ; pwd)
source $CWD/common.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="chplvis"

make chplvis
