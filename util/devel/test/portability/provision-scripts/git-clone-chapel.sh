#!/usr/bin/env bash

set -exuo pipefail

if [ -d chapel ]
then
  echo WARNING: chapel directory already exists, not cloning
else
  GIT_REPO_URL=${GIT_REPO_URL:-https://github.com/chapel-lang/chapel}

  git_cmd=(git clone --reference-if-able "${REPO_CACHE_PATH:-/missing}/chapel.git" --depth 1 "$GIT_REPO_URL")
  if [ -n "${GIT_BRANCH-}" ]; then
    git_cmd+=(--branch "$GIT_BRANCH")
  fi
  if [ -n "${GIT_REVISION-}" ]; then
    git_cmd+=(--revision "$GIT_REVISION")
  fi
  "${git_cmd[@]}"
fi
