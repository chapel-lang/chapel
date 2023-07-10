#!/usr/bin/env bash
#
# Test hellos with chpl built with jemalloc allocator

CWD=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)
source $CWD/common.bash

export CHPL_HOST_MEM="jemalloc"
export CHPL_NIGHTLY_TEST_CONFIG_NAME="host-jemalloc"

$result = $CWD/nightly -cron -hellos ${nightly_args}
if [ $result -ne 0 ]
then
      echo "Test failed " 
      exit 1
else
      echo "Test suceeded"
fi
