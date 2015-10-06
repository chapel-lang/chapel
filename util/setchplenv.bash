# Bash shell script to set the Chapel environment variables

# Directory of setchplenv.bash script, will not work if script is a symlink
DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )

# Shallow test to see if we are in the correct directory
# Just probe to see if we have a few essential subdirectories --
# indicating that we are probably in a Chapel root directory.
chpl_home=$( cd $DIR/../ && pwd )
if [ ! -d "$chpl_home/util" ] || [ ! -d "$chpl_home/compiler" ] || [ ! -d "$chpl_home/runtime" ] || [ ! -d "$chpl_home/modules" ]; then
    # Chapel home is assumed to be one directory up from setenvchpl.bash script
    echo "Error: \$CHPL_HOME is not where it is expected"
    return
fi

    echo -n "Setting CHPL_HOME "
    export CHPL_HOME=$chpl_home
    echo "to $CHPL_HOME"

    MYPATH=`$CHPL_HOME/util/config/fixpath.py "$PATH"`
    MYMANPATH=`$CHPL_HOME/util/config/fixpath.py "$MANPATH"`

    echo -n "Setting CHPL_HOST_PLATFORM "
    export CHPL_HOST_PLATFORM=`"$CHPL_HOME"/util/chplenv/chpl_platform.py`
    echo "to $CHPL_HOST_PLATFORM"

    echo -n "Updating PATH to include "
    export PATH="$CHPL_HOME"/bin/$CHPL_HOST_PLATFORM:"$CHPL_HOME"/util:"$MYPATH"
    echo "$CHPL_HOME"/bin/$CHPL_HOST_PLATFORM
    echo    "                     and ""$CHPL_HOME"/util

    echo -n "Updating MANPATH to include "
    export MANPATH="$CHPL_HOME"/man:"$MYMANPATH"
    echo "$CHPL_HOME"/man
