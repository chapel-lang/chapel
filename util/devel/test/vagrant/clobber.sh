#!/bin/bash

# make clobber in each VM's chapel directory

./tryit.sh "cd chapel && export GMAKE=`which gmake` && export MAKE=${GMAKE:-make} && $MAKE clobber"
