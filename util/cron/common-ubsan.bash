#!/usr/bin/env bash
#
# Configure environment for testing AddressSanitizer.

UTIL_CRON_DIR=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)

export CHPL_TARGET_MEM=cstdlib
export CHPL_HOST_MEM=cstdlib
export CHPL_SANITIZE=undefined
export CHPL_RT_NUM_THREADS_PER_LOCALE_QUIET=yes
