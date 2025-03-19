#!/usr/bin/env bash
#
# Configure environment for arkouda testing on hpe-apollo-hdr

PYTEST_TEMP_DIR=/hpelustre/chapelu/arkouda-scratch/$CHPL_NIGHTLY_TEST_CONFIG_NAME
rm -rf $PYTEST_TEMP_DIR; mkdir -p $PYTEST_TEMP_DIR
export CHPL_TEST_ARKOUDA_PYTEST_OPTIONS="--temp-directory=$PYTEST_TEMP_DIR"
export ARKOUDA_DEFAULT_TEMP_DIRECTORY=$PYTEST_TEMP_DIR
