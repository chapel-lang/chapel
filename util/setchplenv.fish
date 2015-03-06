# fish shell script to set the Chapel environment variables

# shallow test to see if we are in the correct directory
# Just probe to see if we have a few essential subdirectories --
# indicating that we are probably in a Chapel root directory.
if [ ! -d "util" -o ! -d "compiler" -o ! -d "runtime" -o ! -d "modules" ]
	echo "Error: You must use '. util/setchplenv' from within the chapel root directory."
	exit
end

eval set MYPATH (./util/config/fixpath "$PATH" " ")
eval set MYMANPATH (./util/config/fixpath "$MANPATH" " ")

if [ (count $MYPATH) = 0 ]
  echo "Error running ./util/config/fixpath"
  exit
end

echo -n "Setting CHPL_HOME "
set -x CHPL_HOME $PWD
echo "to $CHPL_HOME"

echo -n "Setting CHPL_HOST_PLATFORM "
set -x CHPL_HOST_PLATFORM (eval $CHPL_HOME/util/chplenv/chpl_platform.py)
echo "to $CHPL_HOST_PLATFORM"

echo -n "Updating PATH "
set -x PATH "$CHPL_HOME/bin/$CHPL_HOST_PLATFORM" "$CHPL_HOME/util" $MYPATH
echo "to include $CHPL_HOME/bin/$CHPL_HOST_PLATFORM"
echo "                     and $CHPL_HOME/util"

echo -n "Updating MANPATH "
set -x MANPATH "$CHPL_HOME"/man "$MYMANPATH"
echo "to include $CHPL_HOME/man"
