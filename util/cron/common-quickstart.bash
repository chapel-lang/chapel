#!/usr/bin/env bash
#
# Configure environment for quickstart testing. This should be sourced by other
# scripts that wish to make use of the variables set here.

source $(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)/common.bash
source ${CHPL_HOME}/util/quickstart/setchplenv.bash

# Skip $CHPL_HOME/test/regex/elliot/I-fail-without-re2
export CHPL_TEST_REGEX=none
