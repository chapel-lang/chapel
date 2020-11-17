#!/usr/bin/env bash

CWD=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)
venv_path=$(python "$CWD/../chplenv/chpl_home_utils.py" --chpldeps)

register_argcomplete="register-python-argcomplete"
if [[ "$venv_path" != "none" && -d "$venv_path" ]]; then
  export PYTHONPATH="$venv_path":$PYTHONPATH
  register_argcomplete="$venv_path/bin/$register_argcomplete"
fi

# Register any python scripts that use argparse/argcomplete
if [ -x "$(command -v $register_argcomplete)" ]; then
  eval "$($register_argcomplete start_test)"
fi
