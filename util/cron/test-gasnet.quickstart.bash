#!/usr/bin/env bash
#
# Test quickstart + gasnet configuration on full suite on linux64.

CWD=$(cd $(dirname $0) ; pwd)
source $CWD/common.bash
source $CWD/common-quickstart.bash
source $CWD/common-gasnet.bash # must come after quickstart source

export CHPL_NIGHTLY_TEST_CONFIG_NAME="gasnet.quickstart"

export GASNET_QUIET=Y

$CWD/nightly -cron -multilocale
