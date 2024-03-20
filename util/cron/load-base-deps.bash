#!/usr/bin/env bash

# Sources base dependencies required for all tests.
# Eventually this should be loading everything via spack.

if [ -f /data/cf/chapel/chpl-deps/setup_chpl_deps.bash ] ; then
  # for chapcs/chapvm, just load all dependencies via spack
  source /data/cf/chapel/chpl-deps/setup_chpl_deps.bash
elif [ "$(hostname -s)" == "osprey" ]; then
  # ditto for osprey
  if [ -f /cray/css/users/chapelu/chpl-deps/load_chpl_deps.bash ] ; then
    source /cray/css/users/chapelu/chpl-deps/load_chpl_deps.bash
  fi
else
  # For our internal testing, this is necessary to get the latest version of gcc
  # on the system.
  if [ -z "${CHPL_SOURCED_BASHRC}" -a -f ~/.bashrc ] ; then
      source ~/.bashrc
      export CHPL_SOURCED_BASHRC=true
  fi

  # load llvm
  if [ -f /cray/css/users/chapelu/setup_system_llvm.bash ] ; then
    source /cray/css/users/chapelu/setup_system_llvm.bash
  elif [ -f /cy/users/chapelu/setup_system_llvm.bash ] ; then
    source /cy/users/chapelu/setup_system_llvm.bash
  fi

  # load cmake
  if [ -f /cray/css/users/chapelu/setup_cmake_nightly.bash ] ; then
    source /cray/css/users/chapelu/setup_cmake_nightly.bash
  elif [ -f /cy/users/chapelu/setup_cmake_nightly.bash ] ; then
    source /cy/users/chapelu/setup_cmake_nightly.bash
  fi
fi
