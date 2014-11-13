#!/bin/sh

if [ -f SKIPIF ]
then
  if [ -x SKIPIF ]
  then
    skip=`./SKIPIF`
    if [ "1" == "$skip" -o "True" == "$skip" ]
    then
      exit 1
    fi
  else
    if [ "1" == `$CHPL_HOME/util/test/testEnv SKIPIF` ]
    then
      exit 1
    fi
  fi
fi

if [ -f $1 ]
then
  if [ -x $1 ]
  then
    skip = `$1`
    if [ "1" == "$skip" -o "True" == "$skip" ]
    then
      exit 1
    fi
  else
    if [ "1" == `$CHPL_HOME/util/test/testEnv $1` ]
    then
      exit 1
    fi
  fi
fi

exit 0
