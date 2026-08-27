#!/usr/bin/env bash

# Check if full (util/setchplenv + GMP/RE2) Chapel build passes make check
# Prints summary at the end

set -exuo pipefail

VAGRANT_DIR=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)

source $VAGRANT_DIR/provision_scripts/chapel-setmakej.sh

cd $VAGRANT_DIR && ./tryit.sh 'bash -c '\''cd chapel && source util/setchplenv.bash && export CHPL_GMP=bundled && export CHPL_RE2=`./util/devel/test/portability/vagrant/re2-supported.py` && export GMAKE=`which gmake` && export MAKE=${GMAKE:-make} && $MAKE '$MAKEJ '&& $MAKE '$MAKEJ' check'\'
