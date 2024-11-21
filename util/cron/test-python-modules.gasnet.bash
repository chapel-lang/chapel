#!/bin/bash
#
# Test generated Python modules

set -e

echo "Starting common setup"

CWD=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)
source $CWD/common-gasnet.bash
source $CWD/common-python-interop.bash

echo "Setting environment variables"
export CHPL_LIB_PIC=pic
export CHPL_LLVM=none

# ensure we fail early if ZMQ is not present
echo "Ensure ZMQ is installed"
cCompiler=`PYTHONPATH=$CHPL_HOME/util/chplenv python3 $CWD/getCompiler.py`
echo "cCompiler is $cCompiler"

zmqDirname="zmqDir-XXXXXX"
actualDirname=`mktemp -d ${zmqDirname}`
actualFilename="$actualDirname/zmq-test.c"
echo '#include "zmq.h"' > $actualFilename
echo 'int main (void) { return 0; }' >> $actualFilename

$cCompiler $actualFilename -lzmq

rm -r $actualDirname

# Testing config changes
echo "Setting Nightly variables"
export CHPL_NIGHTLY_TEST_CONFIG_NAME="python-modules.gasnet"
export CHPL_NIGHTLY_TEST_DIRS="interop/python/multilocale"

$CWD/nightly -cron -futures
