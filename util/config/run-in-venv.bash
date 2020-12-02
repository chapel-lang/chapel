#!/usr/bin/env bash
# Active the virtual environment and run the command supplied
# usage: ./run-in-venv prog [args]

if [ -z "$CHPL_HOME" ]; then
  # compute the chpl home directory
  export CHPL_HOME=$(cd $(dirname $0) ; cd ..; cd ..; pwd)
fi

python=$($CHPL_HOME/util/config/find-python.sh)
chpldeps=$("$python" "$CHPL_HOME/util/chplenv/chpl_home_utils.py" --chpldeps)

if [ ! -e "$chpldeps" ]; then
  echo "chpl dependencies are missing - try make test-venv" 1>&2
  exit 1
fi

if [ ! -f "$chpldeps/__main__.py" ]; then
  echo "chpl dependencies are missing - try make test-venv" 1>&2
  exit 1
fi

# include the dependencies
export PYTHONPATH="$chpldeps":$PYTHONPATH
exec "$1" "${@:2}"
