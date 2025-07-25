#!/bin/sh

if [ -d chapel ]
then
  echo WARNING: chapel directory already exists, not cloning
else
  git clone --reference-if-able "${REPO_CACHE_PATH:-/missing}/chapel.git" --depth 1 --branch=vagrant-automation https://github.com/arifthpe/chapel
fi
