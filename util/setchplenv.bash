# bash shell script to set the Chapel environment variables


# shallow test to see if we are in the correct directory
path_tail=`echo $PWD | sed 's/.*\///g'`
if [ "$path_tail" != "chapel" ] 
   then
      echo "Error: source util/setchplenv from within the chapel directory"
   else
      echo -n "Setting CHPL_HOME "
      export CHPL_HOME=$PWD
      echo "to $CHPL_HOME"

      echo -n "Setting CHPL_PLATFORM "
      export CHPL_PLATFORM=`"$CHPL_HOME"/util/platform`
      echo "to $CHPL_PLATFORM"

      echo -n "Updating PATH to include "
      export PATH="$PATH":"$CHPL_HOME"/bin/$CHPL_PLATFORM
      echo "$CHPL_HOME"/bin/$CHPL_PLATFORM

      echo -n "Updating MANPATH to include "
      export MANPATH="$MANPATH":"$CHPL_HOME"/man
      echo "$CHPL_HOME"/man
fi
