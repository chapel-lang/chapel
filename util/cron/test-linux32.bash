#!/usr/bin/env bash
#
# Test default configuration against examples on 32bit linux.

CWD=$(cd $(dirname $0) ; pwd)
source $CWD/common.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="linux32"

suppression_file=$CWD/../../test/Suppressions/linux32.suppress
$CWD/nightly -cron -no-futures -suppress ${suppression_file}
