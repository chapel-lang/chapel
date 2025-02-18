#!/bin/sh

# Check a command, $1, to see if its a valid executable
# if it is, print it and exit, otherwise do nothing
function check_python {
  if command -v $1 > /dev/null 2>&1
  then
    if $1 --version > /dev/null 2>&1
    then
      echo $1
      exit 0
    fi
  fi
}

if [ -n "$CHPL_PYTHON" ]; then
  check_python $CHPL_PYTHON
  echo "python executable '$CHPL_PYTHON' not found" 1>&2
  exit 1
fi

for cmd in python3 python python2; do
  check_python $cmd
done

echo "python3 not found - please install python3 and add it to PATH" 1>&2
# in case the result of this script is used anyway, we want
# an error to mention python3
echo python3
exit 1
