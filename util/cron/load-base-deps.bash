#!/usr/bin/env bash

# Sources base dependencies required for all tests.

# For most systems, load all dependencies via spack
if [[ "${HOSTNAME:0:6}" == "chapcs" || "${HOSTNAME:0:6}" == "chapvm" ]]; then
  if [ -f /hpcdc/project/chapel/chpl-deps/chapcs11/load_chpl_deps.bash ] ; then
    source /hpcdc/project/chapel/chpl-deps/chapcs11/load_chpl_deps.bash
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
elif [[ "$(hostname -s)" == "horizon" ]]; then
  if [ -f /lus/scratch/chapelu/chpl-deps/horizon/load_chpl_deps.bash ] ; then
    source /lus/scratch/chapelu/chpl-deps/horizon/load_chpl_deps.bash
  fi
elif [[ "$(hostname -s)" == "chapel-rocky-9" ]]; then
  # TODO: set up spack for this machine instead of manual installs
  PATH="$HOME/chapelu/Python-3.11.13-install/bin:$PATH"
else
  # For systems not using a Spack install

  # load llvm
  if [ -f /hpcdc/project/chapel/setup_llvm.bash ] ; then
    source /hpcdc/project/chapel/setup_llvm.bash
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
