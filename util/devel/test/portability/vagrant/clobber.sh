#!/bin/bash

set -exuo pipefail

./tryit.sh "cd chapel && export GMAKE=`which gmake` && export MAKE=${GMAKE:-make} && $MAKE clobber"
