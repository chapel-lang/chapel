#!/usr/bin/env bash

#--paint-with ./stream.plot.paint.py

export CHPLEXP_MAX_LOCALES=256
set -x -e
"$CHPL_HOME/util/test/chplExperiment" \
   --skip-if-config-error  \
   \
  `#name           features  options          command`                         \
  `#-------------------------------------------------------------------------` \
   chpl            comm                      ./ra.plot.gather.bash             \
   mpi             mpi       --no-build-chpl ./ra.plot.gather.mpi.bash
