#!/usr/bin/env bash

# 2017-11-03: needed on chapelmac since I.T. updates on 2017-10-10
#             known to be needed on a Macbook when connected over VPN
export CHPL_RT_MASTERIP=127.0.0.1
export CHPL_RT_WORKERIP=127.0.0.0

# We run darwin testing heavily oversubscribed, so limit the number of Chapel
# executables that run concurrently to avoid timeouts.
export CHPL_TEST_LIMIT_RUNNING_EXECUTABLES=yes
