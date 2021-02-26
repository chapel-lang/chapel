#!/bin/sh

for cmd in python3 python python2; do
  if command -v $cmd > /dev/null 2>&1
  then
    if $cmd --version > /dev/null 2>&1
    then
      echo $cmd
      exit 0
    fi
  fi
done

echo "python3 not found - please install python3 and add it to PATH" 1>&2
# in case the result of this script is used anyway, we want
# an error to mention python3
echo python3
exit 1
