# fish shell script to set the Chapel environment variables

# Directory of setchplenv script, will not work if script is a symlink
set DIR (cd (dirname (status -f)); pwd)

# Shallow test to see if we are in the correct directory
# Just probe to see if we have a few essential subdirectories --
# indicating that we are probably in a Chapel root directory.
set chpl_home (fish -c "cd $DIR/../../; pwd")
if [ ! -d "$chpl_home/util" -o ! -d "$chpl_home/compiler" -o ! -d "$chpl_home/runtime" -o ! -d "$chpl_home/modules" ]
    echo "Error: \$CHPL_HOME is not where it is expected"
    exit
end

echo -n "Setting CHPL_HOME "
set -x CHPL_HOME $chpl_home
echo "to $CHPL_HOME"

set MYPATH (eval "$CHPL_HOME/util/config/fixpath.py \"$PATH\" \"fish\"")
set MYMANPATH (eval "$CHPL_HOME/util/config/fixpath.py \"$MANPATH\" \"fish\"")

if [ (count $MYPATH) = 0 ]
  echo "Error running \$CHPL_HOME/util/config/fixpath"
  exit
end

echo -n "Setting CHPL_HOST_PLATFORM "
set -x CHPL_HOST_PLATFORM (eval "$CHPL_HOME/util/chplenv/chpl_platform.py")
echo "to $CHPL_HOST_PLATFORM"

echo -n "Updating PATH "
eval set -x PATH $CHPL_HOME/bin/$CHPL_HOST_PLATFORM $CHPL_HOME/util $MYPATH
echo "to include $CHPL_HOME/bin/$CHPL_HOST_PLATFORM"
echo "                     and $CHPL_HOME/util"

echo -n "Updating MANPATH "
eval set -x MANPATH $CHPL_HOME/man $MYMANPATH
echo "to include $CHPL_HOME/man"

echo "Setting CHPL_COMM to none"
set -x CHPL_COMM none

echo "Setting CHPL_TASKS to fifo"
set -x CHPL_TASKS fifo

#echo "Setting CHPL_MEM to cstdlib"
#set -x CHPL_MEM cstdlib

echo "Setting CHPL_GMP to none"
set -x CHPL_GMP none

echo "Setting CHPL_REGEXP to none"
set -x CHPL_REGEXP none

echo "Setting CHPL_LLVM to none"
set -x CHPL_LLVM none
