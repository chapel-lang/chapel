#!/usr/bin/env bash
#
# Configure environment for testing AddressSanitizer.

CWD=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)

export CHPL_MEM=cstdlib
export CHPL_TASKS=fifo
export CHPL_LLVM=none
export CHPL_SANITIZE=address
export ASAN_OPTIONS=detect_leaks=0
export CHPL_RT_NUM_THREADS_PER_LOCALE_QUIET=yes
