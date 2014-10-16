#!/usr/bin/env bash
#
# Test fifo + gasnet configuration on full suite on linux64.

CWD=$(cd $(dirname $0) ; pwd)
source $CWD/common.bash
source $CWD/common-fifo.bash
source $CWD/common-gasnet.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="gasnet.fifo"

$CWD/nightly -cron
