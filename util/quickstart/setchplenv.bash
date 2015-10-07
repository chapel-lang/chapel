# bash shell script to set the Chapel environment variables

# Directory of setchplenv script, will not work if script is a symlink
DIR=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
echo $DIR

# Shallow test to see if we are in the correct directory
# Just probe to see if we have a few essential subdirectories --
# indicating that we are probably in a Chapel root directory.
chpl_home=$( cd $DIR/../../ && pwd )
if [ ! -d "$chpl_home/util" ] || [ ! -d "$chpl_home/compiler" ] || [ ! -d "$chpl_home/runtime" ] || [ ! -d "$chpl_home/modules" ]; then
    # Chapel home is assumed to be one directory up from setenvchpl.bash script
    echo "Error: \$CHPL_HOME is not where it is expected"
    return
fi

export CHPL_HOME=$chpl_home
echo "Setting CHPL_HOME to $CHPL_HOME"

MYPATH=`$CHPL_HOME/util/config/fixpath.py "$PATH"`
MYMANPATH=`$CHPL_HOME/util/config/fixpath.py "$MANPATH"`

export CHPL_HOST_PLATFORM=`"$CHPL_HOME"/util/chplenv/chpl_platform.py`
echo "Setting CHPL_HOST_PLATFORM to $CHPL_HOST_PLATFORM"

export PATH="$CHPL_HOME"/bin/$CHPL_HOST_PLATFORM:"$CHPL_HOME"/util:"$MYPATH"
echo "Updating PATH to include $CHPL_HOME"/bin/$CHPL_HOST_PLATFORM
echo    "                     and ""$CHPL_HOME"/util

export MANPATH="$CHPL_HOME"/man:"$MYMANPATH"
echo "Updating MANPATH to include $CHPL_HOME"/man

echo "Setting CHPL_COMM to none"
export CHPL_COMM=none

echo "Setting CHPL_TASKS to fifo"
export CHPL_TASKS=fifo

#echo "Setting CHPL_MEM to cstdlib"
#export CHPL_MEM=cstdlib

echo "Setting CHPL_GMP to none"
export CHPL_GMP=none

echo "Setting CHPL_REGEXP to none"
export CHPL_REGEXP=none

echo "Setting CHPL_LLVM to none"
export CHPL_LLVM=none
