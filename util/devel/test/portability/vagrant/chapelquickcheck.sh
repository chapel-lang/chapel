#!/usr/bin/env bash

# Checks if a Quickstart build passes make check
# Prints a summary at the end.

set -exuo pipefail

VAGRANT_DIR=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)

source $VAGRANT_DIR/provision-scripts/chapel-setmakej.sh

cd $VAGRANT_DIR && ./tryit.sh 'bash -c '\''cd chapel && source util/quickstart/setchplenv.bash && export GMAKE=`which gmake` && export MAKE=${GMAKE:-make} && $MAKE '$MAKEJ' && $MAKE '$MAKEJ' check'\'
