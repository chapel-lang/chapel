# csh/tcsh shell script to set the Chapel environment variables

set sourced=($_)
set filedir = `dirname $sourced[2]`

# Directory of setchplenv script, will not work if script is a symlink
set DIR = `cd $filedir && pwd`

# Shallow test to see if we are in the correct directory
# Just probe to see if we have a few essential subdirectories --
# indicating that we are probably in a Chapel root directory.
set chpl_home = `cd $DIR/../../ && pwd`
if ( ! -d "$chpl_home/util" || ! -d "$chpl_home/compiler" || ! -d "$chpl_home/runtime" || ! -d "$chpl_home/modules" ) then
    echo "Error: \$CHPL_HOME is not where it is expected"
    return
endif

echo -n "Setting CHPL_HOME "
setenv CHPL_HOME $chpl_home
echo "to $CHPL_HOME"

set MYPATH = `$CHPL_HOME/util/config/fixpath.py "$PATH"`
set MYMANPATH = `$CHPL_HOME/util/config/fixpath.py "$MANPATH"`

if ( "$MYPATH" == "" ) then
  echo "Error running \$CHPL_HOME/util/config/fixpath"
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

echo "Setting CHPL_COMM to none"
setenv CHPL_COMM none

echo "Setting CHPL_TASKS to fifo"
setenv CHPL_TASKS fifo

#echo "Setting CHPL_MEM to cstdlib"
#setenv CHPL_MEM cstdlib

echo "Setting CHPL_GMP to none"
setenv CHPL_GMP none

echo "Setting CHPL_REGEXP to none"
setenv CHPL_REGEXP none

echo "Setting CHPL_LLVM to none"
setenv CHPL_LLVM none

