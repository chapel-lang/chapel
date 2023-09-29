#!/usr/bin/env bash

set -x -e
"$CHPL_HOME/util/test/chplExperiment" \
   --skip-if-config-error --paint-with ./transpose.plot.paint.py \
   \
  `#name           features  options          command`                         \
  `#-------------------------------------------------------------------------` \
   nvidia          nvidia                    "./transpose.plot.gather.chpl.bash"
