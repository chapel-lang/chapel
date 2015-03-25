#!/usr/bin/env bash
#
# Setup environment for LLVM testing.

CWD=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)

export CHPL_LLVM=llvm

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

# Run examples and test/extern/ferguson/.
export CHPL_NIGHTLY_TEST_DIRS="extern/ferguson"

nightly_args="-llvm"
