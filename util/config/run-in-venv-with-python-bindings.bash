#!/usr/bin/env bash
# Active the virtual environment (including Python bindings) and run the command supplied
# usage: ./run-in-venv-with-python-bindings prog [args]

CWD=$(cd $(dirname $0) ; pwd)

# Perform checks and set up environment variables for virtual env.
source $CWD/run-in-venv-common.bash

chpl_frontend_py_deps=$("$python" "$CHPL_HOME/util/chplenv/chpl_home_utils.py" --chpldeps-version)
export PYTHONPATH="$chpl_frontend_py_deps":$PYTHONPATH

if [ ! -d "$chpl_frontend_py_deps/chapel" ]; then
  python_version=$("$python" -c 'import sys; print(".".join(map(str, sys.version_info[:2])))')
  echo "chapel python bindings are not built for python ${python_version}" 1>&2
  echo "make sure your current python version matches the one used to build chapel" 1>&2
  exit 1
fi

exec "$1" "${@:2}"
