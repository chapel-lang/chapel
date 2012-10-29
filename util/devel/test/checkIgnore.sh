#!/bin/sh

if [ -f SKIPIF ]
then
  if [ "1" == `$CHPL_HOME/util/test/testEnv SKIPIF` ]
  then
    exit 1
  fi
fi

if [ -f $1 ]
then
  if [ "1" == `$CHPL_HOME/util/test/testEnv $1` ]
  then
    exit 1
  fi
fi

exit 0
