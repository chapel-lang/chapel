#!/usr/bin/env bash

# ensure we fail early if ZMQ is not present

set -e

echo "Ensure ZMQ is installed"
cCompiler=`PYTHONPATH=$CHPL_HOME/util/chplenv python3 $UTIL_CRON_DIR/getCompiler.py`
echo "cCompiler is $cCompiler"

zmqDirname="zmqDir-XXXXXX"
actualDirname=`mktemp -d ${zmqDirname}`
actualFilename="$actualDirname/zmq-test.c"
echo '#include "zmq.h"' > $actualFilename
echo 'int main (void) { return 0; }' >> $actualFilename

$cCompiler $actualFilename -lzmq

rm -r $actualDirname
