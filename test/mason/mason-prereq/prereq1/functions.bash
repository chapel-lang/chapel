#!/usr/bin/env bash

# Helper functions for logging and marking test start/stop. Formats output in
# the way the testing infrastructure expects.


function mason_test_start() {
  prev_path=$(echo $PATH)
  # make sure that mason is in $PATH
  export PATH=$(dirname $1):$PATH

  subtest_start_time=$(date +%s)
  echo "[Starting subtest - $(date)]"
}

function mason_test_end() {
  export PATH=$prev_path
  local subtest_end_time=$(date +%s)
  local elapsed=$(( $subtest_end_time - $subtest_start_time ))
  echo "[Finished subtest \"$1\" - $elapsed seconds]"
}

function test_start() {
  subtest_test_name=$1
  echo "[test: \"$subtest_test_name\"]"
  SECONDS=0
}

function test_end() {
  elapsed=$SECONDS
  echo "[Elapsed time to compile and execute all versions of \"$subtest_test_name\" - $elapsed.000 seconds]"
  unset subtest_test_name
}

function log() {
  local msg=$@
  echo "[${msg}]"
}

function test_command() {
  local cmd=$@
  test_start "${cmd}"
  $cmd
  if [[ $? -ne 0 ]] ; then
    log "Error running '${cmd}'"
  else
    log "Success matching '${cmd}'"
  fi
  test_end
}

