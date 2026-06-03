#!/usr/bin/env bash

# a helper function to select python versions before running `nightly` but
# *after* sourcing other `common*.bash`
function set_and_check_python_version() {
  local ver_str=$1

  local major_ver=$(echo $ver_str | cut -d. -f1)
  local minor_ver=$(echo $ver_str | cut -d. -f2)

  # override `python`
  export PATH=/hpcdc/project/chapel/no-python:$PATH

  local setup_script="/hpcdc/project/chapel/chpl-deps/chapcs11/setup_python.bash"

  if [[ -f "${setup_script}" ]] ; then
    source ${setup_script} $major_ver.$minor_ver
  else
    echo "[Error: cannot find the python configuration script: ${setup_script}]"
    exit 2
  fi

  check_python_version $ver_str

  echo "Using $(python3 --version)"
}

# Check correct Python version loaded, exiting with error if not
# ignores the patch version, so 3.9.21 and 3.9.0 would be considered the same version
function check_python_version() {
  local expected_python_version=$1
  local expected_major_minor_version=$(cut -d. -f1,2 <<< "$expected_python_version")

  local actual_python_version=$(python3 --version | cut -d' ' -f2)
  local actual_major_minor_version=$(cut -d. -f1,2 <<< "$actual_python_version")

  if [ "$actual_major_minor_version" != "$expected_major_minor_version" ]; then
    echo "Wrong Python version"
    echo "Expected Version: $expected_python_version. Actual Version: $actual_python_version"
    exit 2
  fi
}
