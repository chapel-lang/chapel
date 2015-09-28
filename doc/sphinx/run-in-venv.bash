#!/usr/bin/env bash
# Active the virtual environment and run the command supplied
# usage: ./run-in-venv prog [args]

if [ -z "$CHPL_HOME" ]; then
  echo "Error: CHPL_HOME is not set"
  exit 1
fi

platform=$("$CHPL_HOME/util/chplenv/chpl_platform.py" --target)
venv_path="$CHPL_HOME/third-party/chpl-venv/install/$platform/chpl-virtualenv/"

if [ ! -d "$venv_path" ]; then
  echo "Error: virtualenv '$venv_path' does not exist"
  exit 1
fi

source "$venv_path/bin/activate"

exec "$1" "${@:2}"
