#!/usr/bin/env bash
#
# Test gasnet (segment everything) against full suite on darwin

CWD=$(cd $(dirname $0) ; pwd)
source $CWD/common-gasnet.bash

# Use relay SMTP server, since postfix does not reliably start when test
# machine is rebooted.
export CHPL_UTIL_SMTP_HOST=relaya

export CHPL_NIGHTLY_TEST_CONFIG_NAME="gasnet.darwin"

# 2017-11-03: needed on chapelmac since I.T. updates on 2017-10-10
#             known to be needed on a Macbook when connected over VPN
export GASNET_MASTERIP=127.0.0.1
export GASNET_WORKERIP=127.0.0.0

$CWD/nightly -cron
