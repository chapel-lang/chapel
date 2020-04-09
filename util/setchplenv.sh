# POSIX-standard compatibility shell script to set the Chapel environment variables
# Source this for POSIX-standard shells such as 'sh' and 'dash'
# Due to POSIX-standard limitations, this must be sourced from $CHPL_HOME


# Shallow test to see if we are in the correct directory
# Just probe to see if we have a few essential subdirectories --
# indicating that we are probably in a Chapel root directory.
if [ ! -d "util" ] || [ ! -d "compiler" ] || [ ! -d "runtime" ] || [ ! -d "modules" ]; then
    echo "Error: You must use '. util/setchplenv.sh' from within the chapel root directory."
    return 1
fi
MYPATH=`./util/config/fixpath.py "$PATH"`
exitcode=$?
MYMANPATH=`./util/config/fixpath.py "$MANPATH"`

# Double check $MYPATH before overwriting $PATH
if [ -z "${MYPATH}" -o "${exitcode}" -ne 0 ]; then
    echo "Error:  util/config/fixpath.py failed"
    echo "        Make sure you have Python 2.5+"
    return 1
fi

echo "Setting CHPL_HOME..."
CHPL_HOME=`pwd`
export CHPL_HOME
echo "                    ...to $CHPL_HOME"
echo " "

CHPL_BIN_SUBDIR=`"$CHPL_HOME"/util/chplenv/chpl_bin_subdir.py`

echo "Updating PATH to include..."
PATH="$CHPL_HOME"/bin/$CHPL_BIN_SUBDIR:"$CHPL_HOME"/util:"$MYPATH"
export PATH
echo "                           ...$CHPL_HOME"/bin/$CHPL_BIN_SUBDIR
echo "                       and ...$CHPL_HOME"/util
echo " "

echo "Updating MANPATH to include..."
MANPATH="$CHPL_HOME"/man:"$MYMANPATH"
export MANPATH
echo "                           ...$CHPL_HOME"/man
echo " "
