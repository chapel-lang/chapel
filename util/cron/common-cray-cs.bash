#!/usr/bin/env bash
#
# Check for, and configure environment for, a Cray CS system.
#  input: $1: 'y' to print a message if this doesn't seem to be a
#             Cray CS system, otherwise silent 
#  output: iff this seems to be a CS, CHPL_HOST_PLATFORM=cray-cs

function loadCSModule()
{
  local m=$@
  if module avail $m 2>&1 | grep -F -q $m ; then
    module load $m
  else
    log_info "This seems to be a Cray CS system, but lacks module $m."
  fi
}

if module avail craype- 2>&1 | grep -q craype- ; then
  export CHPL_HOST_PLATFORM=cray-cs
  export CHPL_TEST_LAUNCHCMD=$CHPL_HOME/util/test/chpl_launchcmd.py
  loadCSModule gcc
  loadCSModule python/2.7.6
  loadCSModule cray-fftw
else
  [ "$1" == y ] && log_info "Expected Cray CS, but does not seem to be one."
fi
