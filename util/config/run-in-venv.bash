#!/usr/bin/env bash
# Active the virtual environment and run the command supplied
# usage: ./run-in-venv prog [args]

if [ -z "$CHPL_HOME" ]; then
  # compute the chpl home directory
  export CHPL_HOME=$(cd $(dirname $0) ; cd ..; cd ..; pwd)
fi

python=$($CHPL_HOME/util/config/find-python.sh)
venv_path=$("$python" "$CHPL_HOME/util/chplenv/chpl_home_utils.py" --venv)

if [ ! -d "$venv_path" ]; then
  echo "Error: virtualenv '$venv_path' does not exist" 1>&2
  exit 1
fi

if [ ! -f "$venv_path/bin/activate" ]; then
  echo "Activation file $venv_path/bin/activate is missing" 1>&2
  exit 1
fi

source "$venv_path/bin/activate"

exec "$1" "${@:2}"
