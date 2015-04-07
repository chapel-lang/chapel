# csh/tcsh shell script to set the Chapel environment variables

# shallow test to see if we are in the correct directory
# Just probe to see if we have a few essential subdirectories --
# indicating that we are probably in a Chapel root directory.
if ( ! -d "util" || ! -d "compiler" || ! -d "runtime" || ! -d "modules" ) then
   echo "Error: source util/setchplenv from within the chapel directory"
   exit
endif

set MYPATH = `./util/config/fixpath "$PATH" :`
set MYMANPATH = `./util/config/fixpath "$MANPATH" :`

if ( "$MYPATH" == "" ) then
  echo "Error running ./util/config/fixpath"
  exit
endif

echo -n "Setting CHPL_HOME "
setenv CHPL_HOME "$cwd"
echo "to $CHPL_HOME"

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

echo "Setting CHPL_LAUNCHER to none"
setenv CHPL_LAUNCHER none

