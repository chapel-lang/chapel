#!/usr/bin/env bash
#
# Configure environment for testing AddressSanitizer.

CWD=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)

export CHPL_MEM=cstdlib
export CHPL_TASKS=fifo
export CHPL_SANITIZE=address
export ASAN_OPTIONS=detect_leaks=0
