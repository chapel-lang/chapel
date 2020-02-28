#!/usr/bin/env bash

# Helper functions for logging and marking test start/stop. Formats output in
# the way the testing infrastructure expects.

function log_error() {
  local msg=$@
  echo "[Error ${msg}]"
  exit 0
}

function log_success() {
  local msg=$@
  echo "[Success matching ${msg}]"
}

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
  local tname=$1
  echo "[test: \"$tname\"]"
  SECONDS=0
}

function test_end() {
  local tname="$1"
  elapsed=$SECONDS
  echo "[Elapsed time to compile and execute all versions of \"$tname\" - $elapsed.000 seconds]"
}
