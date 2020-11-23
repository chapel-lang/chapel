#!/usr/bin/env bash
#
# Test default configuration with `make check`, on linux64, with python 2.7

CWD=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)
source $CWD/common.bash

# set the make flavor to avoid using `util/chplenv/chpl_make.py`
export CHPL_NIGHTLY_MAKE=gmake
export CHPL_NIGHTLY_TEST_CONFIG_NAME="linux64-python27"

# we need to override pythons that were setup by `source .bashrc` in
# common.bash before running nightly. Also see the llvm config for a similar
# trick
py27_setup=/data/cf/chapel/setup_python27.bash
if [ -f "${py27_setup}" ] ; then
  source ${py27_setup}
else
  echo "[Warning: cannot find the python configuration script: ${py27_setup}]"
fi

export PATH=/cray/css/users/chapelu/no-python:$PATH
export PATH=/cray/css/users/chapelu/no-python3:$PATH


$CWD/nightly -cron -python2 ${nightly_args}
