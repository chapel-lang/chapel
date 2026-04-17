#!/bin/sh

if [ -d chapel ]
then
  echo WARNING: chapel directory already exists, not cloning
else
  GIT_BRANCH=${GIT_BRANCH:-main}
  GIT_REPO_URL=${GIT_REPO_URL:-https://github.com/chapel-lang/chapel}
  git clone --reference-if-able "${REPO_CACHE_PATH:-/missing}/chapel.git" --depth 1 --branch="$GIT_BRANCH" "$GIT_REPO_URL"
fi
