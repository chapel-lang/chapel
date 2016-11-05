#!/bin/sh

# takes in a single argument: the preferred version, e.g. 3.7
PREFERRED_VERSION=$1

novers=`which llvm-config`
prefvers=`which llvm-config-$PREFERRED_VERSION`

if [ -x $prefvers ]
then
  echo $prefvers
elif [ -x $novers ]
then
  echo $novers
else
  echo missing-llvm-config
fi
