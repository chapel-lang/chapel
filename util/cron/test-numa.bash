#!/usr/bin/env bash
#
# Test numa locale model with default configuration on full suite.

CWD=$(cd $(dirname $0) ; pwd)
source $CWD/common.bash
source $CWD/common-numa.bash

#
# Force runtime topology to say we have 2 NUMA domains per compute node.
# If we actually do this has no real effect.  If not, in all likelihood
# we have 0 (zero), and we need this to get the expected testing output.
#
export CHPL_RT_NUM_NUMA_DOMAINS=2

export CHPL_NIGHTLY_TEST_DIRS="release/examples/hello*.chpl localeModels"

export CHPL_NIGHTLY_TEST_CONFIG_NAME="numa"

$CWD/nightly -cron
