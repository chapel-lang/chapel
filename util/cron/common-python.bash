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
  export PATH=/hpcdc/project/chapel/no-python:$PATH

  local setup_script="/hpcdc/project/chapel/setup_python$major_ver$minor_ver.bash"

  if [[ -f "${setup_script}" ]] ; then
    source ${setup_script}
  else
    echo "[Warning: cannot find the python configuration script: ${setup_script}]"
    return 1
  fi

  echo "Using $(python3 --version)"
}

# Check correct Python version loaded, exiting with error if not
function check_python_version() {
  local expected_python_version=$1

  local actual_python_version=$(python3 --version | cut -d' ' -f2)

  if [ "$actual_python_version" != "$expected_python_version" ]; then
    echo "Wrong Python version"
    echo "Expected Version: $expected_python_version. Actual Version: $actual_python_version"
    exit 2
  fi
}
