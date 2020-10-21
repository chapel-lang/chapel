#!/bin/sh

for cmd in python3 python python2; do
  command -v > /dev/null $cmd && echo $cmd && exit 0
done

echo "python3 not found - please install python3 and add it to PATH" 1>&2
# in case the result of this script is used anyway, we want
# an error to mention python3
echo python3
exit 1
