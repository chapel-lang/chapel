#!/usr/bin/env bash

# Check if default (util/setchplenv) Chapel build passes make check
# Prints summary at the end

./tryit.sh 'bash -c '\''cd chapel && source util/setchplenv.bash && export GMAKE=`which gmake` && export MAKE=${GMAKE:-make} && $MAKE && $MAKE check'\'
