#!/usr/bin/env bash

#
# This script checks for an existing Python package and installs it if it is not found.
#
# After this script exits, if the package is not available "True" is printed to stdout.
# If the package is available, "False" is printed to stdout.
#

INSTALL_DIR=$1
shift
PACKAGE=$1
shift
# technically this could be something like `-r requirements.txt` as well
OTHER_PACKAGES=$@

# respect CHPL_TEST_VENV_DIR if it is set and not none
if [ -n "$CHPL_TEST_VENV_DIR" ] && [ "$CHPL_TEST_VENV_DIR" != "none" ]; then
  chpl_python=$CHPL_TEST_VENV_DIR/bin/python3
else
  chpl_python=$($CHPL_HOME/util/config/find-python.sh)
fi

# try and import the first package, if it fails, try and install the packages
PACKAGE_NAME=$(echo $PACKAGE | cut -d'<' -f1 | cut -d'>' -f1 | cut -d'=' -f1 | cut -d'!' -f1)

if ! $chpl_python -c "import $PACKAGE_NAME" &>/dev/null; then
  echo "[Attempting to install $PACKAGE_NAME ($PACKAGE)]"
  MY_LIB_DIR=$INSTALL_DIR/python_libs
  $chpl_python -m pip install $PACKAGE $OTHER_PACKAGES --target=$MY_LIB_DIR 2>&1
  export PYTHONPATH=$MY_LIB_DIR:$PYTHONPATH
  if ! $chpl_python -c "import $PACKAGE_NAME" &>/dev/null; then
    echo "[Failed to install $PACKAGE_NAME ($PACKAGE)]"
    echo "True"
  else
    echo "[Successfully installed $PACKAGE_NAME ($PACKAGE)]"
    echo "False"
  fi
else
  echo "[Skipping installing $PACKAGE_NAME ($PACKAGE), already installed]"
  echo "False"
fi
