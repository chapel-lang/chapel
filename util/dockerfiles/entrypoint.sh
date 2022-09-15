#!/bin/bash
export CHPL_TARGET_ARCH=`$CHPL_HOME/util/chplenv/chpl_bin_subdir.py --host`
export PATH=$PATH:$CHPL_HOME/bin/$CHPL_TARGET_ARCH:$CHPL_HOME/util
exec "/bin/bash"