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
          echo "Setting CHPL_HOME..."
          CHPL_HOME=`pwd`
          export CHPL_HOME
          echo "                    ...to $CHPL_HOME"
          echo " "

          echo "Setting CHPL_HOST_PLATFORM..."
          CHPL_HOST_PLATFORM=`"$CHPL_HOME"/util/chplenv/chpl_platform.py`
          export CHPL_HOST_PLATFORM
          echo "                        ...to $CHPL_HOST_PLATFORM"
          echo " "

          echo "Updating PATH to include..."
          PATH="$CHPL_HOME"/bin/$CHPL_HOST_PLATFORM:"$CHPL_HOME"/util:"$MYPATH"
          export PATH
          echo "                           ...$CHPL_HOME"/bin/$CHPL_HOST_PLATFORM
          echo "                       and ...$CHPL_HOME"/util
          echo " "

          echo "Updating MANPATH to include..."
          MANPATH="$CHPL_HOME"/man:"$MYMANPATH"
          export MANPATH
          echo "                           ...$CHPL_HOME"/man
          echo " "

          echo "Setting CHPL_COMM to..."
          CHPL_COMM=none
          export CHPL_COMM
          echo "                           ...none"
          echo ""

          echo "Setting CHPL_TASKS to..."
          CHPL_TASKS=fifo
          export CHPL_TASKS
          echo "                           ...fifo"
          echo " "

#          echo "Setting CHPL_MEM to..."
#          CHPL_MEM=cstdlib
#          export CHPL_MEM
#          echo "                           ...cstdlib"
#          echo " "

          echo "Setting CHPL_GMP to..."
          CHPL_GMP=none
          export CHPL_GMP
          echo "                           ...none"
          echo " "

          echo "Setting CHPL_REGEXP to..."
          CHPL_REGEXP=none
          export CHPL_REGEXP
          echo "                           ...none"
          echo " "
          
        fi
   else
      echo "Error: You must use '. util/setchplenv' from within the chapel root directory."
fi
