#!/usr/bin/env bash

CWD=$(cd $(dirname $0) ; pwd)
source $CWD/functions.bash
source $CWD/common-knc.bash

# Number of logical processes on current system. Will be used as number of jobs
# when calling make with parallel execution.
num_procs=$(python -c "import multiprocessing; print(multiprocessing.cpu_count())")

# Ensure it builds!
log_info "Calling make..."
make -j${num_procs} -C $CWD/../..
log_info "Finished building chpl for knc."
