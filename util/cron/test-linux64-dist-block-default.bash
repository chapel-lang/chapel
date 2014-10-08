#!/usr/bin/env bash
#
# Test block distribution using default configuration.

CWD=$(cd $(dirname $0) ; pwd)
source $CWD/common.bash
source $CWD/common-dists.bash

$CWD/nightly -cron -dist block
