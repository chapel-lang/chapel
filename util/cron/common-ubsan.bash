#!/usr/bin/env bash
#
# Configure environment for testing UndefinedBehaviorSanitizer.

UTIL_CRON_DIR=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)

export CHPL_TARGET_MEM=cstdlib
export CHPL_HOST_MEM=cstdlib
export CHPL_SANITIZE=undefined
# disable licm, since this can result in false positives
# e.g.: x < max(int) && x + 2, if `x + 2` is hoisted there will be an overflow
# but its harmless because the result is never used.
export CHPL_LOOP_INVARIANT_CODE_MOTION=false
