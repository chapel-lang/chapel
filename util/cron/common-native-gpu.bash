CWD=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)

source $CWD/common.bash

# Use latest system LLVM, to use an earlier version uncomment and pass version
# number as a parameter to the script.
# source /cray/css/users/chapelu/setup_system_llvm.bash

# Prevent which LLVM from being set by this variable, so it can be overridden
# later to use ROCM LLVM.
unset CHPL_LLVM_CONFIG
export CHPL_LLVM=system
export CHPL_LOCALE_MODEL=gpu
export CHPL_TEST_GPU=true
export CHPL_NIGHTLY_TEST_DIRS="gpu/native users/engin/context"

unset CHPL_START_TEST_ARGS
nightly_args="${nightly_args} -no-buildcheck"
