#!/usr/bin/env bash

# Check if default (util/setchplenv) Chapel build passes make check
# Prints summary at the end

set -exuo pipefail

VAGRANT_DIR=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)

source $VAGRANT_DIR/provision_scripts/chapel-setmakej.sh

cd $VAGRANT_DIR && ./tryit.sh 'bash -c '\''cd chapel && source util/setchplenv.bash && export GMAKE=`which gmake` && export MAKE=${GMAKE:-make} && $MAKE '$MAKEJ' && $MAKE '$MAKEJ' check'\'
