#!/usr/bin/env bash

# a helper function to select python versions before running `nightly` but
# *after* sourcing other `common*.bash`
function set_python_version() {
  local ver_str=$1

  # make sure the length is OK
  if [[ ${#ver_str} -lt 3 ]] || [[ ${#ver_str} -gt 4 ]]; then
    echo "Python version string must be in format 'x.y[z]'"
    return 1
  fi

  # make sure the 2nd char is "."
  if [[ "${ver_str:1:1}" != "." ]]; then
    echo "Python version string must be in format 'x.y[z]'"
    return 1
  fi

  local major_ver=${ver_str:0:1}
  local minor_ver=${ver_str:2}

  # override `python`
  export PATH=/cray/css/users/chapelu/no-python:$PATH

  if [[ $major_ver -eq 2 ]]; then
    # Do some special casing for python2:
    # 1. Avoid using chpl_make.py and other python scripts as much as possible
    # 2. Override `python3`

    export CHPL_NIGHTLY_MAKE=gmake
    export PATH=/cray/css/users/chapelu/no-python3:$PATH
  fi

  local setup_script="/cray/css/users/chapelu/setup_python$major_ver$minor_ver.bash"

  if [[ -f "${setup_script}" ]] ; then
    source ${setup_script}
  else
    echo "[Warning: cannot find the python configuration script: ${setup_script}]"
    return 1
  fi

  if [[ $major_ver -eq 2 ]]; then
    echo "Using $(python2 --version 2>&1)"
  else
    echo "Using $(python3 --version)"
  fi
}


