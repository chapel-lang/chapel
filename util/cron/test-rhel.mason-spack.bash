#!/usr/bin/env bash
#
# Test mason-spack integration (only) on RHEL linux64 slaves
# The SKIPIF will make sure mason external and spack have been installed and configured

CWD=$(cd $(dirname $0) ; pwd)
source $CWD/common.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="rhel.mason-spack"

export CHPL_NIGHTLY_TEST_DIRS=mason/spack-integration

$CWD/nightly -cron 
