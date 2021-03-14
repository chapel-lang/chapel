#!/usr/bin/env bash

# Helper functions for logging and marking test start/stop. Formats output in
# the way the testing infrastructure expects.

function subtest_start() {
  subtest_start_time=$(date +%s)
  echo "[Starting subtest - $(date)]"
}

function subtest_end() {
  local subtest_end_time=$(date +%s)
  local elapsed=$(( $subtest_end_time - $subtest_start_time ))
  echo "[Finished subtest \"studies/arkouda\" - $elapsed seconds"
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

function log_error() {
  local msg=$@
  echo "[Error ${msg}]"
}

function log_fatal_error() {
  local msg=$@
  echo "[Error ${msg}]"
  if [[ -n $subtest_test_name ]]; then
    test_end
  fi
  subtest_end
  exit 0
}

function log_success() {
  local msg=$@
  echo "[Success matching ${msg}]"
}
