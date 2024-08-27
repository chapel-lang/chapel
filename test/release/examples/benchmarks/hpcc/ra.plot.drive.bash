#!/usr/bin/env bash

#--paint-with ./stream.plot.paint.py

export CHPLEXP_MAX_LOCALES=64
set -x -e
"$CHPL_HOME/util/test/chplExperiment" \
   --paint-with ./ra.plot.paint.py \
   --skip-if-config-error  \
   \
  `#name           features  options          command`                         \
  `#-------------------------------------------------------------------------` \
   chpl_rmo        comm      --var CHPLEXP_USE_ON=false                        \
                                             ./ra.plot.gather.bash             \
                                                                               \
   chpl_on         comm      --var CHPLEXP_USE_ON=true                         \
                                             ./ra.plot.gather.bash             \
                                                                               \
   chpl_atomics    comm      --var CHPLEXP_USE_ATOMICS=true                    \
                                             ./ra.plot.gather.bash             \
                                                                               \
   mpi             mpi       --no-build-chpl ./ra.plot.gather.mpi.bash
