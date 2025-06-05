UTIL_CRON_DIR=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)

source $UTIL_CRON_DIR/common.bash

# Use latest system LLVM, to use an earlier version uncomment and pass version
# number as a parameter to the script.
# source /hpcdc/project/chapel/setup_llvm.bash

export CHPL_LLVM=system
export CHPL_LOCALE_MODEL=gpu
export CHPL_TEST_GPU=true
export CHPL_NIGHTLY_TEST_DIRS="gpu/native users/engin/context release/examples"

unset CHPL_START_TEST_ARGS
