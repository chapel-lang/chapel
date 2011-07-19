# bash shell script to set the Chapel environment variables


# shallow test to see if we are in the correct directory
# Just probe to see if we have a few essential subdirectories --
# indicating that we are probably in a Chapel root directory.
if [ -d "util" ] && [ -d "compiler" ] && [ -d "runtime" ] && [ -d "modules" ]
   then
      echo "Setting CHPL_HOME..."
      CHPL_HOME=`pwd`
      export CHPL_HOME
      echo "                    ...to $CHPL_HOME"
      echo " "

      echo "Setting CHPL_HOST_PLATFORM..."
      CHPL_HOST_PLATFORM=`"$CHPL_HOME"/util/chplenv/platform`
      export CHPL_HOST_PLATFORM
      echo "                        ...to $CHPL_HOST_PLATFORM"
      echo " "

      echo "Updating PATH to include..."
      PATH="$PATH":"$CHPL_HOME"/bin/$CHPL_HOST_PLATFORM
      export PATH
      echo "                           ...$CHPL_HOME"/bin/$CHPL_HOST_PLATFORM
      echo " "

      echo "Updating MANPATH to include..."
      MANPATH="$MANPATH":"$CHPL_HOME"/man
      export MANPATH
      echo "                           ...$CHPL_HOME"/man
      echo " "
   else
      echo "Error: You must use '. util/setchplenv' from within the chapel root directory."
fi
