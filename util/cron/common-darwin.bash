#!/usr/bin/env bash


# Use relay SMTP server, since postfix does not reliably start when test
# machine is rebooted.
export CHPL_UTIL_SMTP_HOST=relaya

# 2017-11-03: needed on chapelmac since I.T. updates on 2017-10-10
#             known to be needed on a Macbook when connected over VPN
export GASNET_MASTERIP=127.0.0.1
export GASNET_WORKERIP=127.0.0.0

# We run darwin testing heavily oversubscribed, so limit the number of Chapel
# executables that run concurrently to avoid timeouts.
export CHPL_TEST_LIMIT_RUNNING_EXECUTABLES=yes
