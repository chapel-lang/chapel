#!/bin/bash

if [ ! -f configure ]
then
  echo "Should be run from a CHPL_HOME directory" 1>&2
  exit 1
fi

unset CHPL_HOME
export CHPL_CHECK_HOME=`pwd`
wd=`pwd`

mytmpdir=`mktemp -d 2>/dev/null || mktemp -d -t 'mytmpdir'`
myprefix="$mytmpdir/prefix"
myhome="$mytmpdir/chplhome"

mkdir -p "$myprefix"
mkdir -p "$myhome"

EXITSTATUS=0

if [ $EXITSTATUS -eq 0 ]
then
  # First, check installation to bin lib etc
  ./configure --prefix="$myprefix" && make && make install

  # Remove bin and lib to eliminate possible confusion
  rm -Rf bin lib

  # Check that bin/chpl was created
  if [ ! -f "$myprefix/bin/chpl" ]
  then
    echo "--prefix install: $myprefix/bin/chpl was not installed!" 1<&2
    EXITSTATUS=1
  else
    # Try compiling and running a test program
    (
      export PATH=$myprefix/bin:$PATH
      $CHPL_CHECK_HOME/util/test/checkChplInstall
      exit $?
    )
    TEST_STATUS=$?
    if [ $TEST_STATUS -ne 0 ]
    then
      echo "--prefix install: running test executable failed" 1>&2
      EXITSTATUS=$TEST_STATUS
    fi
  fi
fi

if [ $EXITSTATUS -eq 0 ]
then
  # Next, check installation to a chpl-home
  ./configure --chpl-home="$myhome" && make && make install
  binsubdir=`./util/chplenv/chpl_bin_subdir.py`

  # Remove bin and lib to eliminate possible confusion
  rm -Rf bin lib

  # Check that chpl-home/bin/subdir/chpl was created
  if [ ! -f "$myhome/bin/$binsubdir/chpl" ]
  then
    echo "--chpl-home install: $myhome/bin/$binsubdir/chpl was not installed!" 1<&2
    EXITSTATUS=1
  else
    # Try compiling and running a test program
    (
      export PATH=$myhome/bin/$binsubdir:$PATH
      $CHPL_CHECK_HOME/util/test/checkChplInstall
      exit $?
    )
    TEST_STATUS=$?
    if [ $TEST_STATUS -ne 0 ]
    then
      echo "--chpl-home install: test executable failed" 1>&2
      EXITSTATUS=$TEST_STATUS
    fi
  fi
fi

rm -Rf $myprefix
rm -Rf $myhome

if [ $EXITSTATUS -eq 0 ]
then
  echo test_install: SUCCESS
else
  echo test_install: FAILURE
fi

exit $EXITSTATUS
