#!/usr/bin/env bash
#
# Test default configuration with comm=gasnet against examples on mac os x.

CWD=$(cd $(dirname $0) ; pwd)
source $CWD/common-gasnet.bash

# Use relay SMTP server, since postfix does not reliably start when test
# machine is rebooted.
export CHPL_UTIL_SMTP_HOST=relaya

export CHPL_NIGHTLY_TEST_CONFIG_NAME="gasnet.darwin"

$CWD/nightly -cron -examples
