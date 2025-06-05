#!/usr/bin/env bash
# Active the virtual environment and run the command supplied
# usage: ./run-in-venv prog [args]

CWD=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)

# Perform checks and set up environment variables for virtual env.
source $CWD/run-in-venv-common.bash

# Make sure that chpldeps (via its __main__.py) is installed.
if [ ! -f "$chpldeps/__main__.py" ]; then
  echo "chpl dependencies are missing - try make test-venv" 1>&2
  exit 1
fi

exec "$1" "${@:2}"
