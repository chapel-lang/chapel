#!/usr/bin/env bash
# Active the virtual environment and run the command supplied
# usage: ./run-in-venv prog [args]

if [ -z "$CHPL_HOME" ]; then
  # compute the chpl home directory
  export CHPL_HOME=$(cd $(dirname $0) ; cd ..; cd ..; pwd)
fi

python=$($CHPL_HOME/util/config/find-python.sh)
venv_dir=$("$python" "$CHPL_HOME/util/chplenv/chpl_home_utils.py" --venv)

if [ ! -d "$venv_dir" ]; then
  echo "Error: virtualenv '$venv_dir' does not exist" 1>&2
  exit 1
fi

if [ ! -f "$venv_dir/bin/python3" ]; then
  echo "python3 wrapper file $venv_dir/bin/python3 is missing" 1>&2
  exit 1
fi

# these steps correspond to what a venv activate script contains
export VIRTUAL_ENV="$venv_dir"
export PATH="$VIRTUAL_ENV/bin:$PATH"
unset PYTHONHOME

exec "$1" "${@:2}"
