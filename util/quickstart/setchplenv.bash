# bash shell script to set the Chapel environment variables


# shallow test to see if we are in the correct directory
# Just probe to see if we have a few essential subdirectories --
# indicating that we are probably in a Chapel root directory.
if [ -d "util" ] && [ -d "compiler" ] && [ -d "runtime" ] && [ -d "modules" ]
   then
      MYPATH=`./util/config/fixpath "$PATH" :`
      MYMANPATH=`./util/config/fixpath "$MANPATH" :`
      if [ -z "$MYPATH" ]
        then
          echo "Error running ./util/config/fixpath";
        else
          echo -n "Setting CHPL_HOME "
          export CHPL_HOME=$PWD
          echo "to $CHPL_HOME"

          echo -n "Setting CHPL_HOST_PLATFORM "
          export CHPL_HOST_PLATFORM=`"$CHPL_HOME"/util/chplenv/chpl_platform.py`
          echo "to $CHPL_HOST_PLATFORM"

          echo -n "Updating PATH to include "
          export PATH="$CHPL_HOME"/bin/$CHPL_HOST_PLATFORM:"$CHPL_HOME"/util:"$MYPATH"
          echo "$CHPL_HOME"/bin/$CHPL_HOST_PLATFORM
          echo    "                     and ""$CHPL_HOME"/util

          echo -n "Updating MANPATH to include "
          export MANPATH="$CHPL_HOME"/man:"$MYMANPATH"
          echo "$CHPL_HOME"/man

          echo "Setting CHPL_COMM to none"
          export CHPL_COMM=none

          echo "Setting CHPL_TASKS to fifo"
          export CHPL_TASKS=fifo

#          echo "Setting CHPL_MEM to cstdlib"
#          export CHPL_MEM=cstdlib

          echo "Setting CHPL_GMP to none"
          export CHPL_GMP=none

          echo "Setting CHPL_REGEXP to none"
          export CHPL_REGEXP=none

          echo "Setting CHPL_LLVM to none"
          export CHPL_LLVM=none

          echo "Setting CHPL_LAUNCHER to none"
          export CHPL_LAUNCHER=none
        fi
   else
      echo "Error: util/setchplenv must be sourced from within the chapel root directory"
fi
