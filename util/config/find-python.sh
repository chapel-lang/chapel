#!/bin/sh

# Check a command, $1, to see if its a valid executable
# if it is, print it and exit, otherwise do nothing
check_python() {
  if command -v $1 > /dev/null 2>&1
  then
    PYTHON_VERSION_OUTPUT=$($1 --version 2>&1)
    if [ $? -eq 0 ]
    then
      MIN_MINOR_PYTHON_VERSION=5
      if printf "Python 3.%s\n%s\n" "$MIN_MINOR_PYTHON_VERSION" "$PYTHON_VERSION_OUTPUT" | sort -V -C
      then
        echo $1
        exit 0
      else
        vers=$($1 --version 2>&1)
        echo "Chapel requires Python 3.$MIN_MINOR_PYTHON_VERSION or later, but found $vers" 1>&2
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
