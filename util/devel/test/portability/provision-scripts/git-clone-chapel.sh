#!/usr/bin/env bash

set -exuo pipefail

if [ -d chapel ]
then
  echo WARNING: chapel directory already exists, not cloning
else
  GIT_REPO_URL=${GIT_REPO_URL:-https://github.com/chapel-lang/chapel}
  GIT_COMMIT=${GIT_COMMIT:-HEAD}

  git_cmd=(git clone --reference-if-able "${REPO_CACHE_PATH:-/missing}/chapel.git" --depth 1 "$GIT_REPO_URL")
  if [ -n "${GIT_BRANCH-}" ]; then
    git_cmd+=(--branch "$GIT_BRANCH")
  fi
  "${git_cmd[@]}"

  pushd chapel && git reset --hard "$GIT_COMMIT" && popd
fi
