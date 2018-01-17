#!/usr/bin/env bash
#
# Test gasnet (segment fast) against hellos on darwin

CWD=$(cd $(dirname $0) ; pwd)
source $CWD/common-gasnet.bash
export CHPL_GASNET_SEGMENT=fast
source $CWD/common-darwin.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="gasnet.fast.darwin"

$CWD/nightly -cron -hellos
