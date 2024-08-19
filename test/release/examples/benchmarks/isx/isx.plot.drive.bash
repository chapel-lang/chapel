#!/usr/bin/env bash

#--paint-with ./stream.plot.paint.py

set -x -e
export CHPLEXP_MAX_LOCALES=64
"$CHPL_HOME/util/test/chplExperiment" \
   --skip-if-config-error  \
   \
  `#name           features  options          command`                         \
  `#-------------------------------------------------------------------------` \
   chpl            comm                      ./isx.plot.gather.bash            \
   shmem           shmem     --no-build-chpl ./isx.plot.gather.shmem.bash
