#!/bin/bash

./tryit.sh "cd chapel && export GMAKE=`which gmake` && export MAKE=${GMAKE:-make} && $MAKE clobber"
