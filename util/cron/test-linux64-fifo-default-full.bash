#!/usr/bin/env bash
#
# Test fifo configuration on full suite on linux64.

CWD=$(cd $(dirname $0) ; pwd)
source $CWD/common.bash
source $CWD/common-fifo.bash

$CWD/nightly -cron
