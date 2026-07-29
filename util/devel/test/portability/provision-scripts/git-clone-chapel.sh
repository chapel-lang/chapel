#!/bin/sh

set -exu

if [ -d chapel ]
then
  echo WARNING: chapel directory already exists, not cloning
else
  GIT_BRANCH_ARG=${GIT_BRANCH:+"--branch $GIT_BRANCH"}
  GIT_REPO_URL=${GIT_REPO_URL:-https://github.com/chapel-lang/chapel}
  GIT_COMMIT=${GIT_COMMIT:-HEAD}
  git clone --reference-if-able "${REPO_CACHE_PATH:-/missing}/chapel.git" --depth 1 "$GIT_BRANCH_ARG" "$GIT_REPO_URL"
  git reset --hard "$GIT_COMMIT"
fi
