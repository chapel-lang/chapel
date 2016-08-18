#!/usr/bin/env bash
#
# Test default configuration on examples only, on linux64, with compiler gcc-4.9

CWD=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)

export CHPL_NIGHTLY_TEST_CONFIG_NAME="linux64-gcc49.examples"

gcc_setup=/data/cf/chapel/setup_gcc49.bash
if source "${gcc_setup}"
then
    source $CWD/common.bash
    $CWD/nightly -cron -examples ${nightly_args}
else
    echo >&2 "error in ${BASH_SOURCE[0]}: source ${gcc_setup}"
    exit 2
fi
