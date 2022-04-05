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

function apply_patch() {
  local patch_file=$@
  echo "Applying patch ${patch_file}"
  if ! patch -p1 < $patch_file ; then
    log_fatal_error "applying patch"
  else
    echo "[Applied patch successfully]"
  fi
}

function test_compile() {
  local kind=$@

  test_start "make $kind"
  make release MOD=$kind 2> $kind.comp.out.tmp
  local status=$?
  cat $kind.comp.out.tmp

  if [[ $status -ne 0 ]] ; then
    log_fatal_error "compiling ${kind}"
  else
    log_success "make output"
  fi
  test_end

  $CHPL_HOME/util/test/computePerfStats comp-time-$kind $CHPL_TEST_PERF_DIR/$CHPL_TEST_PERF_DESCRIPTION $CHAMPS_GRAPH_PATH/comp-time.perfkeys $kind.comp.out.tmp
  if [[ $? -ne 0 ]] ; then
    log_fatal_error "computing compile time stats for ${kind}"
  fi

  $CHPL_HOME/util/test/computePerfStats emitted-code-size-$kind $CHPL_TEST_PERF_DIR/$CHPL_TEST_PERF_DESCRIPTION $CHAMPS_GRAPH_PATH/emitted-code-size.perfkeys $kind.comp.out.tmp
  if [[ $? -ne 0 ]] ; then
    log_fatal_error "computing emitted code size stats for ${kind}"
  fi
}

function test_run() {
  local kind=$1
  local nl=$2

  test_start "run $kind"
  ./bin/champs_${CHAMPS_VERSION}_$kind -nl $nl -f $CHAMPS_CFG_PATH/$kind.in 2>&1 >$kind.exec.out.tmp

  local status=$?
  cat $kind.exec.out.tmp

  if [[ $status -ne 0 ]] ; then
    log_fatal_error "running ${kind}"
  else 
    log_success "$kind output"
  fi
  test_end

  # execute prediff if it exists
  if [ -f $CHAMPS_GRAPH_PATH/$kind.prediff ] ; then
    $CHAMPS_GRAPH_PATH/$kind.prediff dummy $kind.exec.out.tmp
  fi

  $CHPL_HOME/util/test/computePerfStats exec-time-$kind $CHPL_TEST_PERF_DIR/$CHPL_TEST_PERF_DESCRIPTION $CHAMPS_GRAPH_PATH/$kind.perfkeys $kind.exec.out.tmp
  if [[ $? -ne 0 ]] ; then
    log_fatal_error "computing performance stats for ${kind}"
  fi
}

