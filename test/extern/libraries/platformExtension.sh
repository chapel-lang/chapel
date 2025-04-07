#!/usr/bin/env bash

if [[ "$(uname)" == "Darwin" ]]; then
  echo "dylib"
else
  echo "so"
fi


