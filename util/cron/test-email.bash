#!/usr/bin/env bash
#
# Test hellos with chpl built with jemalloc allocator

CWD=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)
source $CWD/common.bash

export CHPL_HOST_MEM="jemalloc"
export CHPL_NIGHTLY_TEST_CONFIG_NAME="test-email"

$CWD/nightly -cron -hellos ${nightly_args}
if [ $? -ne 0 ] 
   then
     log_error "chpl pidigits.chpl failed to compile" 
     exit 1
   else
     log_info "Compiled pidigits.chpl"  
   fi   
