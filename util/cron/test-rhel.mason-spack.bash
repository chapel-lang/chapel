#!/usr/bin/env bash
#
# Test mason-spack integration (only) on RHEL linux64 slaves
# The SKIPIF will make sure mason external and spack have been installed and configured

UTIL_CRON_DIR=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)
source $UTIL_CRON_DIR/common.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="rhel.mason-spack"

export CHPL_NIGHTLY_TEST_DIRS=mason/spack-integration

$UTIL_CRON_DIR/nightly -cron 
