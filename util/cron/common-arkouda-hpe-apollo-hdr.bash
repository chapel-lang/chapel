#!/usr/bin/env bash
#
# Configure environment for arkouda testing on hpe-apollo-hdr

export CHPL_TEST_ARKOUDA_TEMP_DIR=/hpelustre/chapelu/arkouda-scratch/$CHPL_NIGHTLY_TEST_CONFIG_NAME
rm -rf $CHPL_TEST_ARKOUDA_TEMP_DIR; mkdir -p $CHPL_TEST_ARKOUDA_TEMP_DIR
export CHPL_TEST_ARKOUDA_CORRECTNESS_PYTEST_OPTIONS="--temp-directory=$CHPL_TEST_ARKOUDA_TEMP_DIR --timeout=2700"
export ARKOUDA_DEFAULT_TEMP_DIRECTORY=$CHPL_TEST_ARKOUDA_TEMP_DIR
export ARKOUDA_DEP_DIR=/hpelustre/chapelu/arkouda-deps
