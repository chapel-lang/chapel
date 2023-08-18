#!/usr/bin/env bash

set -x -e
"$CHPL_HOME/util/test/chplExperiment" \
   --paint-with ./shootout.plot.paint.py \
   \
  `#name           features  options          command`                         \
  `#-------------------------------------------------------------------------` \
   flat_locales    ""                         ./shootout.plot.gather.bash      \
                                                                               \
   gpu             nvidia  --prebuild "export CHPL_NO_CHECKS=n"                \
                                              ./shootout.plot.gather.bash      \
                                                                               \
   gpu_w_spec      nvidia  --prebuild "export CHPL_NO_CHECKS=n; export CHPL_GPU_SPECIALIZATION=y" \
                                              ./shootout.plot.gather.bash
