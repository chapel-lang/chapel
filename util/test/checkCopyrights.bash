#!/usr/bin/env bash
#
# Verify that all source files have copyright comment in header.
#
# Check that files matching the following patterns have copyright in their
# sources. This check does not necessary ensure these are comments within the
# first few lines of the file.
#
#  *.c
#  *.cc
#  *.chpl
#  *.cpp
#  *.g
#  *.h
#  *.lex
#  *.y
#  *.ypp
#  Make*
#

# Skip check if we are within a grace period at the beginning of a new year.
grace_period_end="01-07"
# Get today's date and the grace period end date as %m%d format for comparison.
todate=`date "+%m%d"`
# Invoke macOS and Linux `date` differently to get the same output.
if [[ "$OSTYPE" == "darwin"* ]]; then
  cond=`date -j -f "%m-%d" $grace_period_end "+%m%d"`
else
  # The hardcoded year is just a placeholder. Date accepts a full date like this,
  # but %m%d cuts off the year so it only compares 0107 to the current date.
  cond=`date -d 1970-$grace_period_end "+%m%d"`
fi

if [ $todate -le $cond ];
then
  echo "[INFO] Skipping copyright check as we are within the grace period (ending $grace_period_end)"
  exit 0;
fi

CWD=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)
CHPL_HOME=${CHPL_HOME:-$CWD/../..}

this_year=$(date '+%Y')
copyright_pattern="Copyright .*${this_year}[^0-9]* Hewlett Packard Enterprise Development LP"
outdated_copyright_pattern="Copyright .* Hewlett Packard Enterprise Development LP"

source_dirs="compiler frontend runtime make modules* util/cmake"

echo "[INFO] Moving to CHPL_HOME: ${CHPL_HOME}"
cd $CHPL_HOME

echo "[INFO] Checking for copyrights in source files: ${copyright_pattern}"

# Check the top-level "source" subdirs under CHPL_HOME ("$source_dirs")
# Filename suffixes actually used, by subdir:
#   compiler: c cpp h lex ypp
#   runtime:  c cc h H
#   modules:  chpl h
#   ALL:      Make*

files_wo_copy=$(find $source_dirs -type f \( \
    -name Make\* -o \
    -name CMakeLists.txt -o \
    -name \*.cmake -o \
    -name \*.cmake.in -o \
    -name \*.c -o \
    -name \*.cc -o \
    -name \*.chpl -o \
    -name \*.cpp -o \
    -name \*.cxx -o \
    -name '*.[hH]' -o \
    -name \*.lex -o \
    -name \*.ypp \) -print | \
    grep -v compiler/codegen/reservedSymbolNames.h | \
    grep -v compiler/include/bison-chapel.h        | \
    grep -v compiler/include/flex-chapel.h         | \
    grep -v compiler/parser/bison-chapel.cpp       | \
    grep -v frontend/lib/parsing/bison-chpl-lib.h | \
    grep -v frontend/lib/parsing/bison-chpl-lib.cpp | \
    grep -v frontend/lib/parsing/flex-chpl-lib.h | \
    grep -v frontend/lib/parsing/flex-chpl-lib.cpp | \
    grep -v frontend/lib/util/git-version.cpp | \
    grep -v 'modules/standard/gen/.*/ChapelSysCTypes.chpl' | \
    grep -v 'modules/packages/ImageHelper/stb/.*.h' | \
    xargs grep -i -L "${copyright_pattern}")


# Check the top-level Makefiles in CHPL_HOME
root_files_wo_copy=$(find . -maxdepth 1 -name Make\* -o -name CMakeLists.txt | xargs grep -i -L "${copyright_pattern}")


# Check CHPL_HOME/tools. Filename suffixes actually used, by subdir:
#   tools/c2chapel (excluding c2chapel/test/, c2chapel/utils/): py
#   tools/chplvis: cxx fl h H
#   tools/mason:   (excluding files named test*): chpl

tools_wo_copy=$(find tools \( -type d \( -name test -o -name utils \) -prune \) -o \( -type f \( \
    -name Make\* -o \
    -name CMakeLists.txt -o \
    -name \*.c -o \
    -name \*.cc -o \
    -name \*.chpl -o \
    -name \*.cpp -o \
    -name \*.cxx -o \
    -name '*.[hH]' -o \
    -name \*.fl -o \
    -name \*.py -o \
    -name c2chapel \) \
    ! -name 'test*' -print \) | \
    xargs grep -i -L "${copyright_pattern}")

if [ -n "${files_wo_copy}" -o -n "${root_files_wo_copy}" -o -n "${tools_wo_copy}" ] ; then
    echo "[ERROR] Missing or incorrect copyrights"
    echo

    files="${files_wo_copy} ${root_files_wo_copy} ${tools_wo_copy}"
    incorrect=$(echo "${files}" | xargs grep -i -l "${outdated_copyright_pattern}")
    missing=$(echo "${files}" | xargs grep -i -L "${outdated_copyright_pattern}")
    if [ -n "${incorrect}" ] ; then
      echo "[ERROR] The following files have incorrect copyrights:"
      echo "${incorrect}"
      echo
      echo " Please correct these by updating the copyright date"
      echo
    fi
    if [ -n "${missing}" ] ; then
      echo "[ERROR] The following files have missing copyrights:"
      echo "${missing}"
      echo
      echo "${missing}" | xargs echo " Add the copyright to these files with \$CHPL_HOME/util/buildRelease/add_license_to_sources.py "
    fi
    exit 1
fi
