#!/usr/bin/env bash
#
# Configure environment for testing valgrind.

CWD=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)

# qthreads doesn't work with valgrind, so use fifo and limit running threads to
# stay below valgrind's --max-threads option, which defaults to 500
source $CWD/common-fifo.bash
export CHPL_RT_NUM_THREADS_PER_LOCALE=450

# jemalloc doesn't work with valgrind, so use cstdlib
export CHPL_MEM=cstdlib

# re2 has opt-in support for valgrind, so enable it
export CHPL_RE2_VALGRIND_SUPPORT=true
