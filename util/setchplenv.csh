# csh/tcsh shell script to set the Chapel environment variables

# shallow test to see if we are in the correct directory
# Just probe to see if we have a few essential subdirectories --
# indicating that we are probably in a Chapel root directory.
if ( ! -d "util" || ! -d "compiler" || ! -d "runtime" || ! -d "modules" ) then
   echo "Error: source util/setchplenv from within the chapel directory"
   exit
endif

echo -n "Setting CHPL_HOME "
setenv CHPL_HOME "$cwd"
echo "to $CHPL_HOME"

echo -n "Setting CHPL_HOST_PLATFORM "
setenv CHPL_HOST_PLATFORM `"$CHPL_HOME/util/chplenv/platform"`
echo "to $CHPL_HOST_PLATFORM"

echo -n "Updating PATH "
setenv PATH "$PATH":"$CHPL_HOME"/bin/$CHPL_HOST_PLATFORM
echo "to include $CHPL_HOME/bin/$CHPL_HOST_PLATFORM"

echo -n "Updating MANPATH "
setenv MANPATH "$MANPATH":"$CHPL_HOME"/man
echo "to include $CHPL_HOME/man"
