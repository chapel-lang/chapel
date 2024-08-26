#!/usr/bin/env bash

set -x -e
export CHPLEXP_MAX_LOCALES=64
"$CHPL_HOME/util/test/chplExperiment" \
   --paint-with ./isx.plot.paint.py \
   --skip-if-config-error  \
   \
  `#name           features  options          command`                         \
  `#-------------------------------------------------------------------------` \
   chpl_1lpn       comm      --var CHPLEXP_NUM_SUBLOCALES=1                    \
                                             ./isx.plot.gather.bash            \
                                                                               \
   chpl_2lpn       comm      --var CHPLEXP_NUM_SUBLOCALES=2                    \
                                             ./isx.plot.gather.bash            \
   shmem           shmem     --no-build-chpl ./isx.plot.gather.shmem.bash
