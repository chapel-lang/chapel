#!/bin/sh

if [ -d chapel ]
then
  echo WARNING: chapel directory already exists, not cloning
else
  reference_repo=/cray/css/users/chapelu/cached_repos/chapel.git
  git clone --reference-if-able $reference_repo --depth 1 https://github.com/chapel-lang/chapel
fi
