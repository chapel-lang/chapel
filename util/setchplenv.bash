# bash shell script to set the Chapel environment variables


# shallow test to see if we are in the correct directory
path_tail=`echo $PWD | sed 's/.*\///g'`
if [ "$path_tail" != "chapel" ] 
   then
      echo "Error: source util/setchplenv from within the chapel directory"
   else
      export CHPL_HOME=$PWD
      export CHPL_PLATFORM=`$CHPL_HOME/util/platform`
      export PATH=$PATH":"$CHPL_HOME"/bin/"$CHPL_PLATFORM
fi



