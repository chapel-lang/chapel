# csh/tcsh shell script to set the Chapel environment variables

# shallow test to see if we are in the correct directory
# Just probe to see if we have a few essential subdirectories --
# indicating that we are probably in a Chapel root directory.
set sourced=($_)
set filedir = `dirname $sourced[2]`
set DIR = `cd $filedir && pwd`

if ( ! -d "$DIR/../util" || ! -d "$DIR/../compiler" || ! -d "$DIR/../runtime" || ! -d "$DIR/../modules" ) then
    echo "Error: \$CHPL_HOME is not where it is expected"
    return
endif

echo -n "Setting CHPL_HOME "
setenv CHPL_HOME "$DIR/../"
echo "to $CHPL_HOME"

set MYPATH = `$CHPL_HOME/util/config/fixpath.py "$PATH"`
set MYMANPATH = `$CHPL_HOME/util/config/fixpath.py "$MANPATH"`

if ( "$MYPATH" == "" ) then
  echo "Error running ./util/config/fixpath"
  exit
endif

echo -n "Setting CHPL_HOST_PLATFORM "
setenv CHPL_HOST_PLATFORM `"$CHPL_HOME/util/chplenv/chpl_platform.py"`
echo "to $CHPL_HOST_PLATFORM"

echo -n "Updating PATH "
setenv PATH "$CHPL_HOME/bin/$CHPL_HOST_PLATFORM":"$CHPL_HOME/util":"$MYPATH"
echo "to include $CHPL_HOME/bin/$CHPL_HOST_PLATFORM"
echo    "                     and $CHPL_HOME/util"

echo -n "Updating MANPATH "
setenv MANPATH "$CHPL_HOME"/man:"$MYMANPATH"
echo "to include $CHPL_HOME/man"
