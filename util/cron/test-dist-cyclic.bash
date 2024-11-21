#!/usr/bin/env bash
#
# Test cyclic distribution using default configuration.

CWD=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)
source $CWD/common-gasnet.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="dist-cyclic"

$CWD/nightly -cron -dist cyclic
