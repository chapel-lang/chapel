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

function update_nightly_repo() {
  pushd $CHAMPS_COMMON_DIR
  git pull
  popd
}

function apply_patch() {
  local patch_file=$@
  echo "[Applying patch ${patch_file}]"
  if ! patch -p1 < $patch_file ; then
    log_fatal_error "applying patch"
  else
    echo "[Applied patch successfully]"
  fi
}

function test_compile() {
  local kind=$@
  local make_vars="MOD=$kind NPROCS=0"
  local reset_chpl_stop_after_pass=false

  if [ ! -z "$CHAMPS_QUICKSTART" ]; then
    local version=quick-shared
    if [ -z "$CHPL_STOP_AFTER_PASS" ]; then
      reset_chpl_stop_after_pass=true
      export CHPL_STOP_AFTER_PASS=denormalize
    fi

    # CHAMPS' Makefile runs `time` in an unportable way causing failures on
    # darwin. Measuring time and memory in this setting is probably useless
    # anyway. So, disable that for quickstart.
    make_vars+=" MONITOR_MAKE=false"

    echo "[Building $kind in quickstart mode. Will stop after the $CHPL_STOP_AFTER_PASS pass]"
  else
    local version=release

    echo "[Building $kind in normal mode]"
  fi

  test_start "make $kind"
  make $version $make_vars 2> $kind.comp.out.tmp
  local status=$?
  cat $kind.comp.out.tmp

  if [[ $status -ne 0 ]] ; then
    log_error "compiling ${kind}"
  else
    log_success "make output"
  fi
  test_end

  # don't leave this environment set
  if [ "$reset_chpl_stop_after_pass" = true ]; then
    unset CHPL_STOP_AFTER_PASS
  fi

  # early return if compilation failed, to prevent spurious perf stats errors
  if [[ $status -ne 0 ]] ; then
    return $status
  fi

  if [ -z "$CHAMPS_QUICKSTART" ]; then
    $CHPL_HOME/util/test/computePerfStats comp-time-$kind $CHPL_TEST_PERF_DIR/$CHPL_TEST_PERF_DESCRIPTION $CHAMPS_GRAPH_PATH/comp-time.perfkeys $kind.comp.out.tmp
    if [[ $? -ne 0 ]] ; then
      log_error "computing compile time stats for ${kind}"
    fi

    $CHPL_HOME/util/test/computePerfStats emitted-code-size-$kind $CHPL_TEST_PERF_DIR/$CHPL_TEST_PERF_DESCRIPTION $CHAMPS_GRAPH_PATH/emitted-code-size.perfkeys $kind.comp.out.tmp
    if [[ $? -ne 0 ]] ; then
      log_error "computing emitted code size stats for ${kind}"
    fi
  fi

  return $status
}

function test_run() {
  local kind=$1
  local nl=$2

  test_start "run $kind"
  # We are setting `CHPL_LAUNCHER_TIMEOUT=pbs, but I needed this `--walltime`.
  # Why?
  eval $CHPL_TEST_LAUNCHCMD --walltime=2:00:00 ./bin/champs_${CHAMPS_VERSION}_$kind -nl $nl -f $kind.in 2>&1 >$kind.exec.out.tmp

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

