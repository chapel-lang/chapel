#!/usr/bin/env bash

# 2017-11-03: needed on chapelmac since I.T. updates on 2017-10-10
#             known to be needed on a Macbook when connected over VPN
export CHPL_RT_MASTERIP=127.0.0.1
export CHPL_RT_WORKERIP=127.0.0.0

# We run darwin testing heavily oversubscribed, so limit the number of Chapel
# executables that run concurrently to avoid timeouts.
export CHPL_TEST_LIMIT_RUNNING_EXECUTABLES=yes


# 2025-10-14: needed on chapelmac since brew updated and no longer puts python3
#             in the PATH by default. this is specific to the version of macos
#             installed, and upgrading the OS may make this unnecessary.
export PATH="$(brew --prefix python3)/libexec/bin:"$PATH
