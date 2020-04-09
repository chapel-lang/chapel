#!/bin/sh

if [ -d chapel ]
then
  echo WARNING: chapel directory already exists, not cloning
else
  git clone --depth 1 https://github.com/chapel-lang/chapel
fi
