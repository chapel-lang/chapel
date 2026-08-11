#!/usr/bin/env bash

set -exuo pipefail

if [ -d chapel ]
then
  echo WARNING: chapel directory already exists, not cloning
else
  git version

  git config --global http.postBuffer 524288000
  git config --global http.maxRequestBuffer 524288000
  git config --global core.compression 0

  GIT_REPO_URL=${GIT_REPO_URL:-https://github.com/chapel-lang/chapel}

  git_cmd=(git clone --reference-if-able "${REPO_CACHE_PATH:-/missing}/chapel.git" --depth 1 "$GIT_REPO_URL")
  if [ -n "${GIT_BRANCH-}" ]; then
    git_cmd+=(--branch "$GIT_BRANCH")
  fi
  "${git_cmd[@]}"
  if [ -n "${GIT_REVISION-}" ]; then
    # Not present on git clone for git<2.49.0, which some of our Vagrant configs
    # use, so fetch it and reset manually after cloning.
    # git_cmd+=(--revision "$GIT_REVISION")
    git fetch origin "$GIT_REVISION:$GIT_REVISION"
    git reset --hard "$GIT_REVISION"
  fi
fi
