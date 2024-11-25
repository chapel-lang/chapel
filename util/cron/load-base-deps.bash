#!/usr/bin/env bash

# Sources base dependencies required for all tests.

# Prevent loading more than once
if [ "$CHPL_BASE_DEPS_LOADED" = "true" ]; then
  echo "Base dependencies already loaded, exiting ${BASH_SOURCE[0]} without reloading them..."
  return 0
fi
export CHPL_BASE_DEPS_LOADED=true

# For most systems, load all dependencies via spack
if [[ "${HOSTNAME:0:6}" == "chapcs" || "${HOSTNAME:0:6}" == "chapvm" ]]; then
  if [ -f /data/cf/chapel/chpl-deps/chapcs11/load_chpl_deps.bash ] ; then
    source /data/cf/chapel/chpl-deps/chapcs11/load_chpl_deps.bash
  fi
elif [[ "$(hostname -s)" == "osprey" || "$(hostname -s)" == "atlas" ||
        "$(hostname -s)" == "pinoak-login1" ]]; then
  if [ -f /lus/scratch/chapelu/chpl-deps/$(hostname -s)/load_chpl_deps.bash ] ; then
    source /lus/scratch/chapelu/chpl-deps/$(hostname -s)/load_chpl_deps.bash
  fi
elif [[ "$(hostname -s)" == "richter-login" ]]; then
  if [ -f /hpelustre/chapelu/chpl-deps/richter-login/load_chpl_deps.bash ] ; then
    source /hpelustre/chapelu/chpl-deps/richter-login/load_chpl_deps.bash
  fi
else
  # For systems not using a Spack install

  # load llvm
  if [ -f /hpcdc/project/chapel/setup_system_llvm.bash ] ; then
    source /hpcdc/project/chapel/setup_system_llvm.bash
  elif [ -f /cy/users/chapelu/setup_system_llvm.bash ] ; then
    source /cy/users/chapelu/setup_system_llvm.bash
  fi

  # load cmake
  if [ -f /hpcdc/project/chapel/setup_cmake_nightly.bash ] ; then
    source /hpcdc/project/chapel/setup_cmake_nightly.bash
  elif [ -f /cy/users/chapelu/setup_cmake_nightly.bash ] ; then
    source /cy/users/chapelu/setup_cmake_nightly.bash
  fi
fi
