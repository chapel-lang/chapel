#!/usr/bin/env bash

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
if ! $chpl_python -c "import $PACKAGE" &>/dev/null; then
  echo "[Attempting to install $PACKAGE]"
  MY_LIB_DIR=$INSTALL_DIR/python_libs
  $chpl_python -m pip install $PACKAGE $OTHER_PACKAGES --target=$MY_LIB_DIR 2>&1
  export PYTHONPATH=$MY_LIB_DIR:$PYTHONPATH
  if ! $chpl_python -c "import $PACKAGE" &>/dev/null; then
    echo "[Failed to install $PACKAGE]"
    echo "True"
  else
    echo "[Successfully installed $PACKAGE]"
    echo "False"
  fi
else
  echo "[Skipping installing $PACKAGE, already installed]"
  echo "False"
fi
