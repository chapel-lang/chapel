#!/bin/bash
export CHPL_TARGET_ARCH=`$CHPL_HOME/util/chplenv/chpl_arch.py --target`
export PATH=$PATH:$CHPL_HOME/bin/linux64-$CHPL_TARGET_ARCH:$CHPL_HOME/util
exec "/bin/bash"