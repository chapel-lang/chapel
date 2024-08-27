#!/usr/bin/env bash

set -x -e
export CHPLEXP_MAX_LOCALES=256
export CHPLEXP_SIZE_GB_PER_NODE=32
"$CHPL_HOME/util/test/chplExperiment"  \
  --paint-with ./stream.plot.paint.py \
  \
  `#name           features  options          command`                         \
  `#-------------------------------------------------------------------------` \
   chpl_1lpn       comm      --var CHPLEXP_NUM_SUBLOCALES=1                    \
                                             ./stream.plot.gather.bash         \
                                                                               \
   chpl_2lpn       comm      --skip-if-colocales-unsupported                   \
                             --var CHPLEXP_NUM_SUBLOCALES=2                    \
                                             ./stream.plot.gather.bash         \
                                                                               \
   mpi             mpi       --no-build-chpl ./stream.plot.gather.mpi.bash
