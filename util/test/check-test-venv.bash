#!/usr/bin/env bash

if [ -z "$CHPL_HOME" ]; then
  # compute the chpl home directory
  export CHPL_HOME=$(cd $(dirname $0) ; cd ..; cd ..; pwd)
fi

python=$($CHPL_HOME/util/config/find-python.sh)
venv_path=$("$python" "$CHPL_HOME/util/chplenv/chpl_home_utils.py" --venv)

if [ -f "$venv_path/chpl-test-reqs" ]
then
  exit 0
else
  make=$("$python" "$CHPL_HOME/util/chplenv/chpl_make.py")
  echo "Chapel test virtualenv not available."\
       "Run a top-level $make test-venv" 1>&2
  exit 1
fi
