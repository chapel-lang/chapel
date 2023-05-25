#!/usr/bin/env bash

if [ -d chapel ]
then
  echo chapel directory already exists - updating
  cd chapel && git checkout main && git pull && cd .. && echo UPDATED
else
  echo cloning chapel
  git clone https://github.com/chapel-lang/chapel && echo CLONED
fi
