#!/usr/bin/env bash
#
# Setup environment for vanilla knc builds/tests.

CWD=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)

# Unload PrgEnv and load vanilla intel compiler.
existing_prgenv=$(module list -t 2>&1 | grep PrgEnv- || :)
if [ -n "${existing_prgenv}" ] ; then
    module unload $existing_prgenv
fi
module load intel

# Unload things that conflict with craype-intel-knc.
knc_conflicts=$(module show craype-intel-knc 2>&1 | \
    grep conflict | sed 's/conflict\s*//')
module unload $knc_conflicts

# Setup environment to build knc.
export CHPL_TARGET_COMPILER=intel
export CHPL_TARGET_ARCH=knc
