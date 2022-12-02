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
  echo "[Finished subtest \"studies/champs\" - $elapsed seconds"
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

function test_compile() {
  test_start "compile"

  chpl -M modules --fast chplGPU.chpl -o chplGPU
  local status=$?
  if [[ $status -ne 0 ]] ; then
    log_fatal_error "compiling"
  else
    log_success "compile output"
  fi

  test_end
}

function test_run() {
  local size=$1
  local testName=chop_$size

  test_start "run $testName"
  ./chplGPU --size=${size} --initial_depth=5 > $testName.exec.out.tmp

  local status=$?
  cat $testName.exec.out.tmp

  if [[ $status -ne 0 ]] ; then
    log_fatal_error "running ${testName}"
  else
    log_success "$testName output"
  fi
  test_end

  # Execute prediff if it exists
  if [ -f $CHOP_GRAPH_PATH/$testName.prediff ] ; then
    $CHOP_GRAPH_PATH/$testName.prediff dummy $testName.exec.out.tmp
  fi

  $CHPL_HOME/util/test/computePerfStats exec-time-$size $CHPL_TEST_PERF_DIR/$CHPL_TEST_PERF_DESCRIPTION $CHOP_GRAPH_PATH/chop.perfkeys $testName.exec.out.tmp
}
