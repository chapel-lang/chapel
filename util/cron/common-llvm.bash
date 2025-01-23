#!/usr/bin/env bash
#
# Setup environment for LLVM testing.
#
# We examine the first commandline parameter,
#   $ source $UTIL_CRON_DIR/common-llvm.bash system # $1 is "system", for example
# if $1 is "bundled" or "system", we set CHPL_LLVM = $1
# all other cases, we set CHPL_LLVM = "bundled" for backward-compatibility

UTIL_CRON_DIR=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)

# set CHPL_LLVM = "bundled" or "system", based on $1 if any
# (default CHPL_LLVM = "bundled", and CHPL_LLVM = "none" is not allowed)

case "$1" in
( bundled | system )
    export CHPL_LLVM=$1
    ;;
( * )
    export CHPL_LLVM=bundled
    ;;
esac

# setup environment based on CHPL_LLVM = "bundled" or "system"

if test "$CHPL_LLVM" = bundled; then

    # 2021-10-26: Developer-installed cmake version required by LLVM 12
    cmake_setup=/hpcdc/project/chapel/setup_cmake_nightly.bash
    if [ -f "${cmake_setup}" ] ; then
        source ${cmake_setup}
    else
        # This is the path to look for on CS systems we have
        cmake_setup=/hpcdc/project/chapel/setup_cmake_nightly.bash
        if [ -f "${cmake_setup}" ] ; then
            source ${cmake_setup}
        else
            echo "[Warning: llvm may not build correctly with cmake: $(which cmake)]"
        fi
    fi
fi

# Run examples and test/extern/ferguson/.
export CHPL_NIGHTLY_TEST_DIRS="extern/ferguson"

export CHPL_TARGET_COMPILER=llvm
