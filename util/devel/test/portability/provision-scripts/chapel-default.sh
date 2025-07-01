#!/usr/bin/env bash

if [ ! -d chapel ]
then
  ../../provision-scripts/chapel-update.sh
fi

source ../../provision-scripts/chapel-setmakej.sh

echo "cd chapel && source util/setchplenv.bash && make $MAKEJ && make check"
cd chapel && source util/setchplenv.bash && make $MAKEJ && make check
