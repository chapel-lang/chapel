#!/usr/bin/env bash
# Active the virtual environment and run the command supplied
# usage: ./run-in-test-venv prog [args]

if [ -z "$CHPL_HOME" ]; then
  # compute the chpl home directory
  export CHPL_HOME=$(cd $(dirname $0) ; cd ..; cd ..; pwd)
fi

python=$($CHPL_HOME/util/config/find-python.sh)
chpldeps=$("$python" "$CHPL_HOME/util/chplenv/chpl_home_utils.py" --chpldeps)

if [ "$CHPL_TEST_VENV_DIR" = "none" ]
then
  echo "Skipping virtualenv activation because CHPL_TEST_VENV_DIR=none."\
       "test-venv requirements must be available."

  # run the command
  exec "$1" "${@:2}"

else
  if [ ! -z "$CHPL_TEST_VENV_DIR" ]
  then
    echo "Using custom virtualenv because CHPL_TEST_VENV_DIR=$venv_dir."\
         "test-venv requirements must be available"

    if [ ! -f "$venv_dir/bin/python3" ]; then
      echo "python3 wrapper file $venv_dir/bin/python3 is missing" 1>&2
      exit 1
    fi

    # now run the command in the activated venv
    source "$CHPL_TEST_VENV_DIR/bin/activate"
    exec "$1" "${@:2}"

  else

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

  fi
fi
