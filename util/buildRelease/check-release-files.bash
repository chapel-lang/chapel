#!/bin/bash
EXITSTATUS=0

SCRIPT=$(readlink -f $0)
FULLTESTNAME=${SCRIPT##*/}
TESTNAME=${FULLTESTNAME%.*}

if [ -z $CHPL_HOME ]
then
  if [ -f "$(dirname $SCRIPT)/../setchplenv.bash" ]
  then
    source "$(dirname $SCRIPT)/../setchplenv.bash"
  else
    echo "$(pwd) $(dirname $SCRIPT)/../setchplenv.bash not found"
    exit 1
  fi
fi

EXCLUSIONS="examples/users-guide/taskpar"

if [ -z $1 ]
then
  $SCRIPT $CHPL_HOME/examples
  EXITSTATUS=$?

  if [ $EXITSTATUS -eq 0 ]
  then
    echo $TESTNAME: SUCCESS
  else
    echo $TESTNAME: FAILURE
  fi
  
  exit $EXITSTATUS
fi

SKIP_PRINT=0

#
# This is a recursive script.
#
# 1) cd to $CHPL_HOME/examples
# 2) Make sure every example chapel program is mentioned in current directory's
#    README file
# 3) Run script on every subdirectory 
# 4) Repeat steps 2 and 3
# 5) Report Success/Fail
#

cd $1
for i in *.chpl
do
  # If no chpl files is current dir, $i will equal '*.chpl'
  if [ ! -r $i ]
  then
    continue
  fi

  # If no README file in current dir, nothing to do
  if [ ! -r README ]
  then
    break
  fi

  # Ignore if we're in an excluded directory
  EXCL_TEST=$(echo $PWD|egrep "$EXCLUSIONS"|wc -l)
  if [ $EXCL_TEST != 0 ]
  then
    if [ $SKIP_PRINT == 0 ]
    then
      echo "Skipping $PWD"
      SKIP_PRINT=1
    fi
    break
  fi

  grep $i README >/dev/null 2>&1
  FOUNDINREADME=$?
  if [ $FOUNDINREADME != 0 ]
  then
    echo "\$CHPL_HOME${PWD#*chapel}/README missing $i"
    EXITSTATUS=$FOUNDINREADME
  fi
done

#
# Run this script on each subdirectory
#
for subdir in *
do
  if [ -d $subdir ]
  then
    $SCRIPT $subdir
    SUBEXITSTATUS=$?
    if [ $SUBEXITSTATUS != 0 ]
    then
      EXITSTATUS=$SUBEXITSTATUS
    fi
  fi
done

exit $EXITSTATUS
