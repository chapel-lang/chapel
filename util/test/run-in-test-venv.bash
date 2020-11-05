#!/usr/bin/env bash
# Active the virtual environment and run the command supplied
# usage: ./run-in-test-venv prog [args]

if [ -z "$CHPL_HOME" ]; then
  # compute the chpl home directory
  export CHPL_HOME=$(cd $(dirname $0) ; cd ..; cd ..; pwd)
fi

python=$($CHPL_HOME/util/config/find-python.sh)
venv_dir=$("$python" "$CHPL_HOME/util/chplenv/chpl_home_utils.py" --test-venv)
def_venv_dir=$("$python" "$CHPL_HOME/util/chplenv/chpl_home_utils.py" --venv)

if [ "$venv_dir" = "none" ]
then
  echo "Skipping virtualenv activation because CHPL_TEST_VENV_DIR=$venv_dir."\
       "test-venv requirements must be available."

else
  if [ "$venv_dir" != "$def_venv_dir" ]
  then
    echo "Using custom virtualenv because CHPL_TEST_VENV_DIR=$venv_dir."\
         "test-venv requirements must be available"

  elif [ ! -f "$venv_dir/chpl-test-reqs" ]
  then
    make=$("$python" "$CHPL_HOME/util/chplenv/chpl_make.py")
    echo "Chapel test virtualenv not available."\
         "Run a top-level $make test-venv" 1>&2
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

fi

# now run the command in the (possibly) activated venv
exec "$1" "${@:2}"
