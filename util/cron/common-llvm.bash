#!/usr/bin/env bash
#
# Setup environment for LLVM testing.
#
# We examine the first commandline parameter,
#   $ source $CWD/common-llvm.bash system # $1 is "system", for example
# if $1 is "llvm" or "system", we set CHPL_LLVM = $1
# all other cases, we set CHPL_LLVM = "llvm" for backward-compatibility

CWD=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)

# set CHPL_LLVM = "llvm" or "system", based on $1 if any
# (default CHPL_LLVM = "llvm", and CHPL_LLVM = "none" is not allowed)

case "$1" in
( llvm | system )
    export CHPL_LLVM=$1
    ;;
( * )
    export CHPL_LLVM=llvm
    ;;
esac

# setup environment based on CHPL_LLVM = "llvm" or "system"

if test "$CHPL_LLVM" = llvm; then

    # Ensure that python 2.7 is at front of PATH. This is only done for
    # llvm configuration because the test systems are _very_ finicky about
    # python 2.6 due to some environmental configurations.
    #
    # Ideally, this would only apply to the `make compile` step in
    # nightly, but there is not a good way to do that right now. One can
    # imagine adding a "pre-make-compile hook" to nightly. Maybe an env
    # var with a comment to executes, but it seems ok for the chpldoc test
    # to fail on llvm for now.
    # (thomasvandoren, 2015-03-11)
     py27_setup=/data/cf/chapel/setup_python27.bash
    if [ -f "${py27_setup}" ] ; then
        source ${py27_setup}
    else
        echo "[Warning: llvm may not build correctly with python: $(which python)]"
    fi

    # 2017-08-10 : Developer-installed cmake version required by LLVM 4
    cmake_setup=/data/cf/chapel/setup_cmake39.bash
    if [ -f "${cmake_setup}" ] ; then
        source ${cmake_setup}
    else
        # This is the path to look for on CS systems we have
        cmake_setup=/cray/css/users/chapelu/setup_cmake39.bash
        if [ -f "${cmake_setup}" ] ; then
            source ${cmake_setup}
        else
            echo "[Warning: llvm may not build correctly with cmake: $(which cmake)]"
        fi
    fi
fi

# Run examples and test/extern/ferguson/.
export CHPL_NIGHTLY_TEST_DIRS="extern/ferguson"

nightly_args="${nightly_args} -llvm"
