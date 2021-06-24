#!/usr/bin/env bash

# Check if full (util/setchplenv + GMP/RE2) Chapel build passes make check
# Prints summary at the end

./tryit.sh 'bash -c '\''cd chapel && source util/setchplenv.bash && export CHPL_GMP=gmp && export CHPL_RE2=`./util/devel/test/vagrant/re2-supported.py` && export GMAKE=`which gmake` && export MAKE=${GMAKE:-make} && $MAKE && $MAKE check'\'
