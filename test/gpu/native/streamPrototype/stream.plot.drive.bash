#!/usr/bin/env bash

set -x -e
"$CHPL_HOME/util/test/chplExperiment" \
   --skip-if-config-error --paint-with ./stream.plot.paint.py \
   \
  `#name           features  options          command`                         \
  `#-------------------------------------------------------------------------` \
   cuda_baseline   nvidia   --no-build-chpl                                    \
                            --skip-if-errs "nvcc --version"                    \
                                             ./stream.plot.gather.cuda.bash    \
                                                                               \
   hip_baseline    amd      --no-build-chpl                                    \
                            --skip-if-errs "hipcc --version"                   \
                                             ./stream.plot.gather.hip.bash     \
                                                                               \
   nvidia          nvidia                    ./stream.plot.gather.chpl.bash    \
                                                                               \
   amd             amd                       ./stream.plot.gather.chpl.bash
