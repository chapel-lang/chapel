# bash shell script to set the Chapel environment variables


# shallow test to see if we are in the correct directory
path_tail=`pwd | sed 's/.*\///g'`
if [ "$path_tail" != "chapel" ] 
   then
      echo "Error: You muse use '. util/setchplenv' from within the chapel directory"
   else
      echo "Setting CHPL_HOME..."
      CHPL_HOME=`pwd`
      export CHPL_HOME
      echo "                    ...to $CHPL_HOME"
      echo " "

      echo "Setting CHPL_HOST_PLATFORM..."
      CHPL_HOST_PLATFORM=`"$CHPL_HOME"/util/platform.pl`
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
fi
