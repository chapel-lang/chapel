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

function log() {
  local msg=$@
  echo "[${msg}]"
}

function apply_git_patch() {
  # Function to apply a git patch to a specified directory

  local PATCH_FILE=$1
  local TARGET_DIR=$2

  # Some checks
  if [[ ! -f "$PATCH_FILE" ]]; then
    log_fatal_error "Patch file '$PATCH_FILE' does not exist."
  fi

  if [[ ! -d "$TARGET_DIR" ]]; then
    log_fatal_error "Target directory '$TARGET_DIR' does not exist."
  fi


  (
    cd "$TARGET_DIR" || log_fatal_error "Failed to change directory to '$TARGET_DIR'."

    # Ensure the directory is a Git repository
    if ! git rev-parse --is-inside-work-tree > /dev/null 2>&1; then
      log_fatal_error "Target directory '$TARGET_DIR' is not a Git repository."
    fi

    # Apply the patch
    echo "Applying patch '$PATCH_FILE' to Git repository in '$TARGET_DIR'..."
    if git apply --check "$PATCH_FILE"; then
      git apply "$PATCH_FILE"
      echo "Patch applied successfully."
    else
      log_fatal_error "Failed to apply patch '$PATCH_FILE'."
    fi

    # Verify
    if git apply --check "$PATCH_FILE" > /dev/null 2>&1; then
      log_fatal_error "Patch application verification failed (patch still applies cleanly)."
    else
      echo "Patch application verified."
    fi
  )
}
