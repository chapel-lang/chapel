#!/bin/sh

# Check a command, $1, to see if its a valid executable
# if it is, print it and exit, otherwise do nothing
check_python() {
  if command -v $1 > /dev/null 2>&1
  then
    if $1 --version > /dev/null 2>&1
    then
      MIN_MINOR_PYTHON_VERSION=5
      if $1 -c "import sys; sys.exit(int(sys.version_info[:2] >= (3, $MIN_MINOR_PYTHON_VERSION)))" >/dev/null 2>&1
      then
        vers=$($1 --version 2>&1)
        echo "Chapel requires Python 3.$MIN_MINOR_PYTHON_VERSION or later, but found $vers" 1>&2
      else
        echo $1
        exit 0
      fi
    fi
  fi
}

if [ -n "$CHPL_PYTHON" ]; then
  check_python $CHPL_PYTHON
  echo "python executable '$CHPL_PYTHON' not found" 1>&2
  exit 1
fi

for cmd in python3 python; do
  check_python $cmd
done

echo "python3 not found - please install python3 and add it to PATH" 1>&2
# in case the result of this script is used anyway, we want
# an error to mention python3
echo python3
exit 1
