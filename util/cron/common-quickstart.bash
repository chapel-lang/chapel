#!/usr/bin/env bash
#
# Configure environment for quickstart testing. This should be sourced by other
# scripts that wish to make use of the variables set here.

CWD=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)
source $CWD/common.bash
source ${CHPL_HOME}/util/quickstart/setchplenv.bash

# Skip $CHPL_HOME/test/regex/elliot/I-fail-without-re2
export CHPL_TEST_REGEX=none
export CHPL_RT_NUM_THREADS_PER_LOCALE_QUIET=yes

# Always use the C backend in quikcstart test configurations.
# quickstart/setchplenv.bash will activate CHPL_LLVM=system if a
# compatible system LLVM is available. However, this testing configuration
# should always use CHPL_LLVM=none because that is a likely possibility
# for quickstart compiles & we test CHPL_LLVM=system in other configs.
export CHPL_LLVM=none
