#!/usr/bin/env bash
# Active the virtual environment (including Python bindings) and run the command supplied
# usage: ./run-in-venv-with-python-bindings prog [args]

CWD=$(cd $(dirname $0) ; pwd)

# Perform checks and set up environment variables for virtual env.
source $CWD/run-in-venv-common.bash

if [ ! -d "$chpldeps/chapel" ]; then
  echo "chapel python bindings not built - try make chapel-py-venv " 1>&2
  exit 1
fi

exec "$1" "${@:2}"
