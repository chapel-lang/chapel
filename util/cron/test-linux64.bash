#!/usr/bin/env bash
#
# Test default configuration on full suite with compiler performance enabled on
# linux64.

CWD=$(cd $(dirname $0) ; pwd)
source $CWD/common.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="linux64"

sed "s/void copyPropagation(void) {/void copyPropagation(void) {\n  return;/" $CHPL_HOME/compiler/optimizations/copyPropagation.cpp  > CP.tmp && mv CP.tmp $CHPL_HOME/compiler/optimizations/copyPropagation.cpp

nightly_args="-compperformance (default)"
$CWD/nightly -cron -futures ${nightly_args}
