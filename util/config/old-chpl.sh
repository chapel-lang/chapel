#!/bin/bash

# Find out filepath depending on shell
if [ -n "${BASH_VERSION}" ]; then
    filepath=${BASH_SOURCE[0]}
elif [ -n "${ZSH_VERSION}" ]; then
    filepath=${(%):-%N}
else
    echo "Error: setchplenv.bash can only be sourced from bash and zsh"
    return 1
fi

# Directory of this script, will not work if script is a symlink
DIR=$(cd "$(dirname "${filepath}")" && pwd)

# Shallow test to see if we are in the correct directory
# Just probe to see if we have a few essential subdirectories --
# indicating that we are probably in a Chapel root directory.
chpl_home=$( cd $DIR/../../ && pwd )
if [ ! -d "$chpl_home/util" ] || [ ! -d "$chpl_home/compiler" ] || [ ! -d "$chpl_home/runtime" ] || [ ! -d "$chpl_home/modules" ]; then
    # Chapel home is assumed to be one directory up from setenvchpl.bash script
    echo "Error: \$CHPL_HOME is not where it is expected"
    echo $chpl_home
    exit 1
fi

oldsub=`$chpl_home/util/chplenv/chpl_platform.py`
newsub=`$chpl_home/util/chplenv/chpl_bin_subdir.py`
oldpath="$chpl_home/bin/$oldsub/chpl"
newpath="$chpl_home/bin/$newsub/chpl"
echo "Please update your PATH" 1>&2
echo " chpl is now stored at $newpath" 1>&2
echo " the old path was $oldpath" 1>&2
$newpath $*
