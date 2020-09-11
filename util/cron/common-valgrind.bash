#!/usr/bin/env bash
#
# Configure environment for testing valgrind.

CWD=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)

# qthreads doesn't work with valgrind, so use fifo and limit running threads to
# avoid "VG_N_THREADS is too low" errors
source $CWD/common-fifo.bash
export CHPL_RT_NUM_THREADS_PER_LOCALE=100

# jemalloc doesn't work with valgrind, so use cstdlib
export CHPL_MEM=cstdlib

# re2 has opt-in support for valgrind, so enable it
export CHPL_RE2_VALGRIND_SUPPORT=true
