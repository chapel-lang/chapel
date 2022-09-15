#!/bin/bash
export CHPL_BIN_SUBDIR=`$CHPL_HOME/util/chplenv/chpl_bin_subdir.py --host`
export PATH=$PATH:$CHPL_HOME/bin/$CHPL_BIN_SUBDIR:$CHPL_HOME/util
exec "$@"