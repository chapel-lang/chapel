#!/usr/bin/env bash

UTIL_CRON_DIR=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)

source $UTIL_CRON_DIR/common.bash

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

# 2026-05-04: Mac test machines keep hitting OOM, so reduce number of concurrent
# make jobs.
export CHPL_MAKE_MAX_CPU_COUNT=4

LATEST_LLVM_SUPPORT=$($UTIL_CRON_DIR/../chplenv/chpl_llvm.py --supported-versions | cut -d, -f1)
ACTUAL_LLVM_VERSION=$($UTIL_CRON_DIR/../chplenv/chpl_llvm.py --llvm-version)
# if system, make sure latest llvm matches the expected
if [ "$($UTIL_CRON_DIR/../chplenv/chpl_llvm.py)" = "system" ]; then
  if [ "$LATEST_LLVM_SUPPORT" != "$ACTUAL_LLVM_VERSION" ]; then
    echo "ERROR: system LLVM version $ACTUAL_LLVM_VERSION does not match expected $LATEST_LLVM_SUPPORT"
    exit 1
  fi
fi
