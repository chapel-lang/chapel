# fish shell script to set the Chapel environment variables

# Directory where this script lives
set DIR (cd (dirname (status -f)); pwd)

# shallow test to see if we are in the correct directory
# Just probe to see if we have a few essential subdirectories --
# indicating that we are probably in a Chapel root directory.
if [ ! -d "$DIR/../util" -o ! -d "$DIR/../compiler" -o ! -d "$DIR/../runtime" -o ! -d "$DIR/../modules" ]
    echo "Error: \$CHPL_HOME is not where it is expected"
    exit
end

echo -n "Setting CHPL_HOME "
set -x CHPL_HOME (fish -c "cd $DIR/../; pwd")
echo "to $CHPL_HOME"

set MYPATH (eval "$CHPL_HOME/util/config/fixpath.py \"$PATH\"")
set MYMANPATH (eval "$CHPL_HOME/util/config/fixpath.py \"$MANPATH\"")

if [ (count $MYPATH) = 0 ]
  echo "Error running ./util/config/fixpath"
  exit
end

echo -n "Setting CHPL_HOST_PLATFORM "
set -x CHPL_HOST_PLATFORM (eval "$CHPL_HOME/util/chplenv/chpl_platform.py")
echo "to $CHPL_HOST_PLATFORM"

echo -n "Updating PATH "
eval set -x PATH $MYPATH $CHPL_HOME/bin/$CHPL_HOST_PLATFORM $CHPL_HOME/util
echo "to include $CHPL_HOME/bin/$CHPL_HOST_PLATFORM"
echo "                     and $CHPL_HOME/util"

echo -n "Updating MANPATH "
eval set -x MANPATH $CHPL_HOME/man $MYMANPATH
echo "to include $CHPL_HOME/man"
