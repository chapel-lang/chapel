#!/usr/bin/env bash
#
# Run performance tests with comm=ofi on a cray-cs system.

CWD=$(cd $(dirname $0) ; pwd)

export CHPL_TEST_PERF_CONFIG_NAME='16-node-cs'

source $CWD/common-perf.bash
export CHPL_TEST_PERF_DIR=/cray/css/users/chapelu/NightlyPerformance/cray-cs/16-node-cs

export CHPL_NIGHTLY_TEST_CONFIG_NAME="perf.cray-cs.ofi"

source $CWD/common-cray-cs.bash y
source $CWD/common-perf-cray-cs.bash
source $CWD/common-ofi.bash

# Stopgap: bump the timeout so all tests pass.  (empty-chpl-remote-taskspawn
# and miniMD time out with the default limit of 300.)
export CHPL_TEST_TIMEOUT=1800

if [[ $($CHPL_HOME/util/chplenv/chpl_platform.py --target) != cray-cs ]] || \
   [[ "$CHPL_COMM_OFI_OOB" != slurm-pmi2 ]] || \
   [[ "$SLURM_MPI_TYPE" != pmix ]] ; then
  log_error "Unexpected environment for Cray CS comm=ofi testing.  Exiting."
  exit 1
fi

export CHPL_RT_MAX_HEAP_SIZE=83G
nightly_args="${nightly_args} -no-buildcheck"
perf_args="-performance-description ofi -numtrials 1"

$CWD/nightly -cron ${perf_args} ${perf_cray_cs_args} ${nightly_args}
