#!/usr/bin/env bash

# get the chpl home directory
FIND_CHPL_HOME=$(cd $(dirname ${BASH_SOURCE[0]}); cd ..; cd ..; pwd)

CHPL_PYTHON=$($FIND_CHPL_HOME/util/config/find-python.sh)
CHPLDEPS=$($CHPL_PYTHON "$FIND_CHPL_HOME/util/chplenv/chpl_home_utils.py" --chpldeps)

register_argcomplete="none"
if [[ "$CHPLDEPS" != "none" && -d "$CHPLDEPS" ]]; then
  register_argcomplete="$CHPL_PYTHON $CHPLDEPS register-python-argcomplete"
else
  if [ -x "$(command -v register-python-argcomplete)" ]; then
    register_argcomplete="register-python-argcomplete"
  fi
fi

# Register any python scripts that use argparse/argcomplete
if [ "$register_argcomplete" != "none" ]; then
  eval "$($register_argcomplete start_test)"
fi
