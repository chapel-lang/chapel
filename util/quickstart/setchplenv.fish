# fish shell script to set the Chapel environment variables

# Directory of setchplenv script, will not work if script is a symlink
set DIRNAME (dirname (status -f))
set DIRPATH (fish -c "cd $DIRNAME; pwd")

# Shallow test to see if we are in the correct directory
# Just probe to see if we have a few essential subdirectories --
# indicating that we are probably in a Chapel root directory.
set chpl_home (fish -c "cd $DIRPATH/../../; pwd")
if [ ! -d "$chpl_home/util" -o ! -d "$chpl_home/compiler" -o ! -d "$chpl_home/runtime" -o ! -d "$chpl_home/modules" ]
    echo "Error: \$CHPL_HOME is not where it is expected"
    exit 1
end

eval set CHPL_PYTHON (eval "$chpl_home/util/config/find-python.sh")

# Remove any previously existing CHPL_HOME paths
eval set MYPATH (eval "$CHPL_PYTHON" "$chpl_home/util/config/fixpath.py \" $PATH \" \"--shell=fish\"")
set exitcode $status
eval set MYMANPATH (eval "$CHPL_PYTHON" "$chpl_home/util/config/fixpath.py \" $MANPATH \" \"--shell=fish\"")

# Double check $MYPATH before overwriting $PATH
if [ (count $MYPATH) = 0 -o ! $exitcode = 0 ]
    echo "Error:  util/config/fixpath.py failed"
    echo "        Make sure you have Python 2.5+"
    exit 1
end

echo -n "Setting CHPL_HOME "
set -x CHPL_HOME $chpl_home
echo "to $CHPL_HOME"

set -x CHPL_BIN_SUBDIR (eval "$CHPL_PYTHON" "$CHPL_HOME/util/chplenv/chpl_bin_subdir.py")

echo -n "Updating PATH "
set -x PATH $CHPL_HOME/bin/$CHPL_BIN_SUBDIR $CHPL_HOME/util $MYPATH
echo "to include $CHPL_HOME/bin/$CHPL_BIN_SUBDIR"
echo "                     and $CHPL_HOME/util"

echo -n "Updating MANPATH "
set -x MANPATH $CHPL_HOME/man $MYMANPATH
echo "to include $CHPL_HOME/man"

echo "Setting CHPL_COMM to none"
set -x CHPL_COMM none

echo "Setting CHPL_TASKS to fifo"
set -x CHPL_TASKS fifo

echo "Setting CHPL_MEM to cstdlib"
set -x CHPL_MEM cstdlib

echo "Setting CHPL_GMP to none"
set -x CHPL_GMP none

echo "Setting CHPL_REGEXP to none"
set -x CHPL_REGEXP none

echo "Setting CHPL_LLVM to none"
set -x CHPL_LLVM none
