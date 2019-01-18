#!/usr/bin/env bash

# Checks if a Quickstart build basses make check
# Prints a summary at the end.

./tryit.sh 'bash -c '\''cd chapel && source util/quickstart/setchplenv.bash && export GMAKE=`which gmake` && export MAKE=${GMAKE:-make} && $MAKE && $MAKE check'\'
