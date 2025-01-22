#!/usr/bin/env bash
# Active the virtual environment (including Python bindings) and run the command supplied
# usage: ./run-in-venv-with-python-bindings prog [args]

CWD=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)

CC=$("$CHPL_HOME/util/printchplenv" --value --only CHPL_HOST_CC)
PLATFORM=$("$CHPL_HOME/util/printchplenv" --value --only CHPL_HOST_PLATFORM)

# Perform checks and set up environment variables for virtual env.
source $CWD/run-in-venv-common.bash

chpl_frontend_py_deps=$("$python" "$CHPL_HOME/util/chplenv/chpl_home_utils.py" --chpldeps-chapel-py)
export PYTHONPATH="$chpl_frontend_py_deps":$PYTHONPATH

if [ ! -d "$chpl_frontend_py_deps/chapel" ]; then
  python_version=$("$python" -c 'import sys; print(".".join(map(str, sys.version_info[:2])))')
  echo "chapel python bindings are not built for python ${python_version}" 1>&2
  # if chpl_frontend_py_deps does not end with 'deps', tell user to match python version
  if [[ "$chpl_frontend_py_deps" != *deps ]]; then
    echo "make sure your current python version matches the one used to build chapel" 1>&2
  fi
  exit 1
fi

if [ $("$python" "$CHPL_HOME/util/chplenv/chpl_sanitizers.py") == "address" ]; then
  if [ "$PLATFORM" == "darwin" ]; then
    echo "Cannot use chapel-py on Mac OS when address sanitization is enabled; please unset 'CHPL_SANITIZE', then rebuild Chapel and chapel-py"
    exit 1
  fi

  if [[ "$CC" == *"clang"* ]]; then
    export LD_PRELOAD=$($CC -print-file-name=libclang_rt.asan.so)
  else
    export LD_PRELOAD=$($CC -print-file-name=libasan.so)
  fi
fi

exec "$1" "${@:2}"
