#!/usr/bin/env bash

#--paint-with ./stream.plot.paint.py

set -x -e
"$CHPL_HOME/util/test/chplExperiment" \
   --skip-if-config-error  \
   \
  `#name           features  options          command`                         \
  `#-------------------------------------------------------------------------` \
   chpl            comm                      ./ig.plot.gather.bash             \
   __skip__shmem           shmem     --no-build-chpl ./ig.plot.gather.shmem.bash
