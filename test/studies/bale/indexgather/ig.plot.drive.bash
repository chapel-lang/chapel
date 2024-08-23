#!/usr/bin/env bash

#--paint-with ./stream.plot.paint.py

set -x -e
export CHPLEXP_MAX_LOCALES=64
"$CHPL_HOME/util/test/chplExperiment" \
   --paint-with ./ig.plot.paint.py \
   --skip-if-config-error  \
   \
  `#name           features  options          command`                         \
  `#-------------------------------------------------------------------------` \
   chpl_fine_1lpn  comm      --var CHPLEXP_NUM_SUBLOCALES=1                    \
                                             ./ig.plot.gather.bash             \
                                                                               \
   chpl_fine_2lpn  comm      --var CHPLEXP_NUM_SUBLOCALES=2                    \
                                             ./ig.plot.gather.bash             \
                                                                               \
   chpl_agg_1lpn   comm      --var CHPLEXP_NUM_SUBLOCALES=1                    \
                                             ./ig.plot.gather.agg.bash         \
                                                                               \
   chpl_agg_2lpn   comm      --var CHPLEXP_NUM_SUBLOCALES=2                    \
                                             ./ig.plot.gather.agg.bash         \
                                                                               \
   shmem                   gnu:shmem --no-build-chpl ./ig.plot.gather.shmem.bash
