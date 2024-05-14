#!/usr/bin/env bash

# Sources base dependencies required for all tests.

# For most systems, load all dependencies via spack
if [[ "${HOSTNAME:0:6}" == "chapcs" || "${HOSTNAME:0:6}" == "chapvm" ]]; then
  if [ -f /data/cf/chapel/chpl-deps/chapcs11/load_chpl_deps.bash ] ; then
    source /data/cf/chapel/chpl-deps/chapcs11/load_chpl_deps.bash
  fi
elif [[ "$(hostname -s)" == "osprey" || "$(hostname -s)" == "atlas" ||
        "$(hostname -s)" == "horizon" || "$(hostname -s)" == "horizon-elogin" ||
        "$(hostname -s)" == "horizon-aarch" ]]; then
  if [ -f /lus/scratch/chapelu/chpl-deps/$(hostname -s)/load_chpl_deps.bash ] ; then
    source /lus/scratch/chapelu/chpl-deps/$(hostname -s)/load_chpl_deps.bash
  fi
elif [[ "$(hostname -s)" == "richter-login" ]]; then
  if [ -f /hpelustre/chapelu/chpl-deps/richter-login/load_chpl_deps.bash ] ; then
    source /hpelustre/chapelu/chpl-deps/richter-login/load_chpl_deps.bash
  fi
else
  # For systems not using a Spack install

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
