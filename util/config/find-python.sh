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
      EXPECTED_ORDER=$(printf "Python 3.%s\n%s\n" "$MIN_MINOR_PYTHON_VERSION" "$PYTHON_VERSION_OUTPUT")

      # note: sort -V is not available on all systems, so we use a workaround: https://stackoverflow.com/a/4495368
      # note: the first key below is not '-n'. This helps with the Python prefix,
      #       but will stop working when Python 10+ is released.
      ACTUAL_ORDER=$(echo "$EXPECTED_ORDER" | sort -t. -k1,1 -k2,2n -k3,3n)
      if [ "$EXPECTED_ORDER" = "$ACTUAL_ORDER" ]
      then
        echo $1
        exit 0
      else
        echo "Chapel requires Python 3.$MIN_MINOR_PYTHON_VERSION or later, but found $PYTHON_VERSION_OUTPUT" 1>&2
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
