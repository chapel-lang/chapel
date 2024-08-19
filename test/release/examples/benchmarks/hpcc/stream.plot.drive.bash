#!/usr/bin/env bash

#--paint-with ./stream.plot.paint.py

set -x -e
export CHPLEXP_MAX_LOCALES=64
"$CHPL_HOME/util/test/chplExperiment" \
   --skip-if-config-error  \
   \
  `#name           features  options          command`                         \
  `#-------------------------------------------------------------------------` \
   chpl            comm                      ./stream.plot.gather.bash         \
   mpi             mpi       --no-build-chpl ./stream.plot.gather.mpi.bash
