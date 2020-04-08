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



############################
## Common Helper Routines ##
############################

# Remove an entire line that contains a given pattern
function removePattern() {
  if [ $# -ne 2 ] || [ ! -f $2 ]; then
    echo "Bad call to removePattern."
    exit 1
  fi
  # remove the line that contains the pattern (assumed to be part of a
  # symbol declaration) and any documentation comments associated with
  # it until the next symbol declaration in the file.
  awk "/$1/{flag=0;next}/\.\. .*:: /{flag=1}flag" $2 > $2.tmp
  mv $2.tmp $2
}

# Replace a given pattern with another
function replace() {
  if [ $# -ne 3 ] || [ ! -f $3 ]; then
    echo "Bad call to replace."
    exit 1
  fi
  sed "s/$1/$2/g" $3 > $3.tmp
  mv $3.tmp $3
}

# Replaces the module title with a given string. For example:
#   `fixTitle 'Synchronization Variables' ChapelSyncvar.rst`
# will replace "module::ChapelSyncvar" with just "Synchronization Variables".
#
# Note: It does not rename the .html file, it will still be ChapelSyncvar.html`
function fixTitle() {
  if [ $# -ne 2 ] || [ ! -f $2 ]; then
    echo "Bad call to fixTitle."
    exit 1
  fi
  local base="$(basename $2 .rst)"
  replace "modules:: $base" "modules:: $1" $2

  # Replace the section header so the internal modules don't show up like this:
  # Module: ChapelFoo
  local titleLen=${#1}
  local header="$(printf '=%.0s' $(seq 1 $titleLen))"
  perl -0777 -i -pe "s/$base\n=+\n/$1\n$header\n/g" $2
}

# Remove unwanted functions:
#  - remove all procs/iters that start with chpl_ (they're internal)
#  - remove all procs/iters that don't start with letters (to remove operator
#    overloads, and functions starting with a underscores
function removePrefixFunctions() {
  if [ $# -ne 1 ] || [ ! -f $1 ]; then
    echo "Bad call to removePrefixFunctions."
    exit 1
  fi
  removePattern "proc [^a-zA-Z]" $1
  removePattern "iter [^a-zA-Z]" $1

  removePattern "proc chpl_" $1
  removePattern "iter chpl_" $1
}

# Remove unwanted variables:
#  - remove variables that start with chpl_
#  - remove variables that start with underscores
function removePrefixVariables() {
  if [ $# -ne 1 ] || [ ! -f $1 ]; then
    echo "Bad call to removePrefixVariables."
    exit 1;
  fi

  removePattern "var _" $1

  removePattern "var chpl_" $1
}

# remove Usage information
function removeUsage() {
  if [ $# -ne 1 ] || [ ! -f $1 ]; then
    echo "Bad call to removeUsage."
    exit 1;
  fi

  sed -e '/\*\*Usage\*\*/ { N; N; N; N; N; N; N; N; N; N; N; N; d; }' $1 > $1.tmp
  mv $1.tmp $1
}

#############################################################
## Modules to fixup listed in INTERNAL_MODULES_TO_DOCUMENT ##
#############################################################

## ChapelTuple ##

file=ChapelTuple.rst
removePrefixFunctions $file
removePattern "param size" $file
removePattern "record:: _tuple" $file
fixTitle "Tuples" $file
removeUsage $file

## ChapelIO ##

file="./ChapelIO.rst"
fixTitle "IO Support" $file
removeUsage $file

## End ChapelIO ##


## ChapelIteratorSupport ##

file="./ChapelIteratorSupport.rst"
removePrefixFunctions $file
fixTitle "Vectorizing Iterator" $file
removeUsage $file


## End ChapelIteratorSupport ##


## ChapelLocale ##

file="./ChapelLocale.rst"
fixTitle "Locales" $file
replace "LocaleSpace = chpl__buildDomainExpr(0..numLocales-1)" \
        "LocaleSpace = {0..numLocales-1}" $file
removeUsage $file


## End ChapelLocale ##


## ChapelSyncvar ##

file="./ChapelSyncvar.rst"
replace "_syncvar" "sync" $file
replace "_singlevar" "single" $file
removePrefixFunctions $file
fixTitle "Synchronization Variables" $file
removeUsage $file

## End ChapelSyncvar ##


## ChapelArray ##
file="./ChapelArray.rst"
replace "_domain" "domain" $file
replace "_array" "array" $file
replace "record" "type" $file

removePrefixFunctions $file
removePrefixVariables $file

fixTitle "Domain and Array Operations" $file
removeUsage $file

## End ChapelArray ##


## Atomics ##

file="./Atomics.rst"

removePrefixFunctions $file

replace "record:: AtomicBool" "type:: atomic \(bool\)" $file
replace "record:: AtomicT"    "type:: atomic \(T\)" $file

fixTitle "Atomics" $file
removeUsage $file

## End Atomics ##

## ChapelRange ##

file="./ChapelRange.rst"

removePrefixFunctions $file
fixTitle "Ranges" $file
removeUsage $file

# End ChapelRange ##

## ChapelComplex_forDocs ##

file="./ChapelComplex_forDocs.rst"

removePrefixFunctions $file
fixTitle "Complex" $file
removeUsage $file
replace "_complex" "complex" $file

# End ChapelComplex_forDocs ##

## String ##

file="./String.rst"
fixTitle "Strings" $file
removeUsage $file

## End of String ##

## Bytes ##

file="./Bytes.rst"
fixTitle "Bytes" $file
removeUsage $file

## End of Bytes ##

## UtilMisc_forDocs ##

file="./UtilMisc_forDocs.rst"

# Don't removePrefixFunctions since it's a stand-in documentation file
# so shouldn't have any that we don't want documented.
fixTitle "Misc Functions" $file
removeUsage $file

# End UtilMisc_forDocs ##

## ChapelEnv ##

file="./ChapelEnv.rst"
fixTitle "Chapel Environment Variables" $file
replace " = AppendExpr.Call09" "" $file
removeUsage $file

## End of ChapelEnv ##

## CPtr ##

file="./CPtr.rst"
removeUsage $file

## End of CPtr ##

## ChapelError ##

file=ChapelError.rst
fixTitle "Errors" $file
removeUsage $file

## End of ChapelError ##

## OwnedObject ##
file=OwnedObject.rst
fixTitle "owned" $file
replace "_owned" "owned" $file
replace "chpl_t" "t" $file
removeUsage $file
## End of OwnedObject ##

## SharedObject ##
file=SharedObject.rst
fixTitle "shared" $file
replace "_owned" "owned" $file
replace "_shared" "shared" $file
replace "chpl_t" "t" $file
removeUsage $file
## End of SharedObject ##
