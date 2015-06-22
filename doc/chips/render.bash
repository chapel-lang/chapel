#!/bin/bash

if [ -z "$CHPL_HOME" ] ; then
    echo "CHPL_HOME must be set to run this script"
    exit 1
fi

PLATFORM=`$CHPL_HOME/util/chplenv/chpl_platform.py`
mkdir -p tmp
cp -r $CHPL_HOME/third-party/chpldoc-venv/chpldoc-sphinx-project/* tmp

rm tmp/source/modules/*.rst
cp *.rst tmp/source/modules

export PATH=$CHPL_HOME/third-party/chpldoc-venv/install/$PLATFORM/chpldoc-virtualenv/bin:$PATH && export VIRTUAL_ENV=$CHPL_HOME/third-party/chpldoc-venv/install/darwin/chpldoc-virtualenv && export CHPLDOC_AUTHOR='' && $CHPL_HOME/third-party/chpldoc-venv/install/$PLATFORM/chpldoc-virtualenv/bin/sphinx-build -b html -d tmp/build/doctrees -W tmp/source ./html

echo point your web browser to html/index.html
echo open html/index.html \# on a mac
echo firefox html/index.html \# on linux
