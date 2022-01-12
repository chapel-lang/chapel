#!/bin/bash

set -e

SOURCE="${BASH_SOURCE[0]}"
while [ -h "$SOURCE" ]; do # resolve $SOURCE until the file is no longer a symlink
  DIR="$( cd -P "$( dirname "$SOURCE" )" >/dev/null && pwd )"
  SOURCE="$(readlink "$SOURCE")"
  [[ $SOURCE != /* ]] && SOURCE="$DIR/$SOURCE" # if $SOURCE was a relative symlink, we need to resolve it relative to the path where the symlink file was located
done
DIR="$( cd -P "$( dirname "$SOURCE" )" >/dev/null && pwd )"

source $DIR/common.sh

if [[ -z "$(which pip)" ]] ; then
    echo failed to find pip
    false
fi

if [[ -z "$(which python)" ]] ; then
    echo failed to find python
    false
fi

function cleanup {
    echo failed to setup environment
}

trap cleanup ERR

if [[ -z "$(which virtualenv)" ]] ; then
    pip --isolated install --root $PREREQ_INSTALL_PATH virtualenv
    export PYTHONPATH=$PREREQ_INSTALL_PATH/usr/lib/python2.7/site-packages:$PYTHONPATH
    export PATH=$PREREQ_INSTALL_PATH/bin:$PATH
fi

if [[ ! -f $VIRTUALENV_INSTALL_PATH/bin/activate ]] ; then
    virtualenv $VIRTUALENV_INSTALL_PATH
fi

source $VIRTUALENV_INSTALL_PATH/bin/activate
pip install -r $DIR/../share/requirements.txt

