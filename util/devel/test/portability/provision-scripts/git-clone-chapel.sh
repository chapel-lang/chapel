#!/usr/bin/env bash

set -exuo pipefail

# Helper function to run a command repeatedly with retries, and a clean command
# between attempts.
function run_cmd_retrying()
{
  CMD=$1
  CLEAN_CMD=$2
  NUM_RETRIES=$3
  RETRY_SLEEP=$4

  i=1
  until [ "$i" -gt "$NUM_RETRIES" ]
  do
    echo "Trying '$CMD' (attempt $i/$NUM_RETRIES)..."
    { eval "$CLEAN_CMD"; } && { eval "$CMD"; } && echo "'$CMD' successful on attempt $i" && break || {
      echo "'$CMD' failed"
      if [ "$i" -lt "$NUM_RETRIES" ]; then
        ((i++))
        if [ "$RETRY_SLEEP" -gt 0 ]; then
          echo "Sleeping $RETRY_SLEEP seconds before retrying"
          sleep $RETRY_SLEEP
        fi
      else
        print_header_message "Could not '$CMD' in $i attempts, giving up"
        exit 1
      fi
    }
  done
}

if [ -d chapel ]
then
  echo WARNING: chapel directory already exists, not cloning
else
  git version

  GIT_REPO_URL=${GIT_REPO_URL:-https://github.com/chapel-lang/chapel}

  if [ -n "${GIT_REVISION}" ]; then
    test -z "${GIT_BRANCH}"
    git init ./chapel
    cd ./chapel
    git remote add origin "$GIT_REPO_URL"

    fetch_cmd="git fetch --filter=blob:none --depth=1 origin $GIT_REVISION:$GIT_REVISION"
    run_cmd_retrying "$fetch_cmd" "" 10 10

    git reset --hard "$GIT_REVISION"
  else
    git_cmd=(git clone --reference-if-able "${REPO_CACHE_PATH:-/missing}/chapel.git" --depth 1 "$GIT_REPO_URL")
    if [ -n "${GIT_BRANCH-}" ]; then
      git_cmd+=(--branch "$GIT_BRANCH")
    fi
    "${git_cmd[@]}"
  fi
fi
