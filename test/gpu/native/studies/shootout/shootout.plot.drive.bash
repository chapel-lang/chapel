#!/usr/bin/env bash

set -x -e
"$CHPL_HOME/util/test/chplExperiment" \
   --paint-with ./shootout.plot.paint.py \
   \
  `#name           features  options          command`                         \
  `#-------------------------------------------------------------------------` \
   01_flat            ""                      ./shootout.plot.gather.bash      \
                                                                               \
   02_flat_gasnet     gasnet  --prebuild "export CHPL_NO_CHECKS=n"             \
                                              ./shootout.plot.gather.bash      \
                                                                               \
   03_gpu_cpu         nvidia  --prebuild "export CHPL_GPU=cpu; export CHPL_NO_CHECKS=n" \
                                              ./shootout.plot.gather.bash      \
                                                                               \
   04_gpu             nvidia  --prebuild "export CHPL_NO_CHECKS=n; export CHPL_FAST=y" \
                                              ./shootout.plot.gather.bash      \
                                                                               \
   05_gpu_w_spec      nvidia  --prebuild "export CHPL_NO_CHECKS=n; export CHPL_GPU_SPECIALIZATION=y" \
                                              ./shootout.plot.gather.bash
