# csh/tcsh shell script to set the Chapel environment variables

# shallow test to see if we are in the correct directory
set path_tail = `echo $cwd | sed 's/.*\///g'`
if ($path_tail != "chapel") then
   echo "Error: source util/setchplenv from within the chapel directory"
   exit
endif

echo -n "Setting CHPL_HOME "
setenv CHPL_HOME "$cwd"
echo "to $CHPL_HOME"

echo -n "Setting CHPL_PLATFORM "
setenv CHPL_PLATFORM `"$CHPL_HOME/util/platform"`
echo "to $CHPL_PLATFORM"

echo -n "Updating PATH "
setenv PATH "$PATH":"$CHPL_HOME"/bin/$CHPL_PLATFORM
echo "to include $CHPL_HOME/bin/$CHPL_PLATFORM"

echo -n "Updating MANPATH "
setenv MANPATH "$MANPATH":"$CHPL_HOME"/man
echo "to include $CHPL_HOME/man"
