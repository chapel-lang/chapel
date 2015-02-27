#!/usr/bin/env bash
#
# Verify that all source files have copyright comment in header.
#
# Check that files matching the following patterns have copyright in their
# sources. This check does not necessary ensure these are comments within the
# first few lines of the file, but that seems excessive.
#
#  *.c
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
copyright_pattern="copyright .*${this_year}.* Cray Inc"
source_dirs="compiler runtime make modules*"

echo "[INFO] Moving to CHPL_HOME: ${CHPL_HOME}"
cd $CHPL_HOME

# COMP: c cpp h lex ypp y g RT: c h ALL: Make* MODULES: chpl


echo "[INFO] Checking for copyrights in source files: ${copyright_pattern}"
files_wo_copy=$(find $source_dirs \
    -name Make\* -o \
    -name \*.c -o \
    -name \*.chpl -o \
    -name \*.cpp -o \
    -name \*.g -o \
    -name \*.h -o \
    -name \*.lex -o \
    -name \*.y -o \
    -name \*.ypp | \
    grep -v compiler/include/bison-chapel.h  | \
    grep -v compiler/include/flex-chapel.h   | \
    grep -v compiler/parser/bison-chapel.cpp | \
    grep -v compiler/parser/flex-chapel.cpp  | \
    grep -v compiler/passes/reservedSymbolNames.h | \
    grep -v modules/standard/gen/ | \
    xargs grep -i -L "${copyright_pattern}")

# Now check the Make* files in CHPL_HOME.
root_files_wo_copy=$(find . -maxdepth 1 -name Make\* | xargs grep -i -L "${copyright_pattern}")

if [ -n "${files_wo_copy}" -o -n "${root_files_wo_copy}" ] ; then
    echo "[ERROR] The following files have missing or incorrect copyrights:"
    echo "${files_wo_copy}"
    echo "${root_files_wo_copy}"
    echo "Add the copyright with: \$CHPL_HOME/util/buildRelease/add_license_to_sources.py <files>"
    exit 1
fi
