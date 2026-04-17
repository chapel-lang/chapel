#!/usr/bin/env sh

#
# This script prints the flags required by the Chapel compiler to embed Python
# code in a Chapel program. It is intended to be used by consumers of the
# 'Python' module. See https://chapel-lang.org/docs/modules/packages/Python.html
#

# get the chpl home directory
chpl_home=$(cd $(dirname $0) ; cd ..; cd ..; pwd)
chpl_python=$("$chpl_home/util/config/find-python.sh")

PYTHON_INCLUDE_DIR=$($chpl_python -c "import sysconfig; print(sysconfig.get_paths()['include'])")
PYTHON_LIB_DIR=$($chpl_python -c "import sysconfig; print(sysconfig.get_config_var('LIBDIR'))")
PYTHON_LDVERSION=$($chpl_python -c "import sysconfig; print(sysconfig.get_config_var('LDVERSION'))")

DISABLE_WARNINGS=""
# some older python's don't use `#ifndef` when they should
# so we disable redefinition warnings for clean testing
DISABLE_WARNINGS="$DISABLE_WARNINGS --ccflags -Wno-macro-redefined"

echo "--ccflags -isystem$PYTHON_INCLUDE_DIR -L$PYTHON_LIB_DIR --ldflags -Wl,-rpath,$PYTHON_LIB_DIR -lpython$PYTHON_LDVERSION $DISABLE_WARNINGS"
