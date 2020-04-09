#!/usr/bin/env bash
#
# Verify that all source files have copyright comment in header.
#
# Check that files matching the following patterns have copyright in their
# sources. This check does not necessary ensure these are comments within the
# first few lines of the file, but that seems excessive.
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

CWD=$(cd $(dirname $0) ; pwd)
CHPL_HOME=${CHPL_HOME:-$CWD/../..}

this_year=$(date '+%Y')
copyright_pattern="Copyright .*${this_year}[^0-9]* Hewlett Packard Enterprise Development LP"
source_dirs="compiler runtime make modules*"

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
    grep -v 'modules/standard/gen/.*/SysCTypes.chpl' | \
    xargs grep -i -L "${copyright_pattern}")


# Check the top-level Makefiles in CHPL_HOME
root_files_wo_copy=$(find . -maxdepth 1 -name Make\* | xargs grep -i -L "${copyright_pattern}")


# Check CHPL_HOME/tools. Filename suffixes actually used, by subdir:
#   tools/c2chapel (excluding c2chapel/test/, c2chapel/utils/): py
#   tools/chplvis: cxx fl h H
#   tools/mason:   (excluding files named test*): chpl

tools_wo_copy=$(find tools \( -type d \( -name test -o -name utils \) -prune \) -o \( -type f \( \
    -name Make\* -o \
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
    echo "[ERROR] The following files have missing or incorrect copyrights:"
    echo "${files_wo_copy}"
    echo "${root_files_wo_copy}"
    echo "${tools_wo_copy}"
    echo "Add the copyright with: \$CHPL_HOME/util/buildRelease/add_license_to_sources.py <files>"
    exit 1
fi
