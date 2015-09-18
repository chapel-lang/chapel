#!/bin/bash


# NOTE:
# This script _should_ eventually go away entirely as chpldoc improves.
# For now, we'll use common unix utilities to achieve a cleaner result.

if [ $# -ne 1 ]; then
  echo "Error: This script takes one argument, the path of the intermediate sphinx project"
  exit 1
fi

if [ ! -d "$1" ]; then
  echo "Error: Unable to find directory $1."
  exit 2
fi

TEMPDIR="$1"

cd "${TEMPDIR}/source/modules/internal/"

function removePattern() {
  sed -i '' "/$1/ { N; d; }" $2
}

function replace() {
  sed -i '' "s/$1/$2/g" $3
}

function fixTitle() {
  sed -i '' "s/module:: $1/module:: $2/g" $3
  # Need to keep the new text short to match length of section marker
  replace "Module: $1" "$2" $3
}

function removePrefixFunctions() {
  removePattern "proc [^a-zA-Z]" $1
  removePattern "proc chpl__" $1
}

## ChapelSyncvar ##

file="./ChapelSyncvar.rst"
replace "_syncvar" "sync" $file
replace "_singlevar" "single" $file
removePrefixFunctions $file
fixTitle "ChapelSyncvar" "Synchronization Vars" $file

## End ChapelSyncvar ##


## Atomics ##
file="./Atomics.rst"

removePattern "type atomic_" $file
removePattern "proc atomic_" $file
removePattern "proc create_" $file
removePrefixFunctions $file

replace "atomic_int64" "atomic\(T\)" $file
replace "int(64)" "T" $file
fixTitle "Atomics" "Atomics" $file

## End Atomics ##
