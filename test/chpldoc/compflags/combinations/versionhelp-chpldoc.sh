#!/usr/bin/env bash
CWD=$(cd $(dirname $0) ; pwd)
compiler=$3

echo -n `basename $compiler`
cat $CWD/../../../compflags/bradc/printstuff/version.goodstart
# During pre-release mode
diff $CWD/../../../../compiler/main/BUILD_VERSION $CWD/zero.txt > /dev/null 2>&1 && echo "" || \
 { echo -n " pre-release (" && cat $CWD/../../../../compiler/main/BUILD_VERSION | tr -d \"\\n && echo ")" ; }
# During release mode:
# echo ""

# print Shpinx and chapeldomain version from pip
PIP="$CWD/../../../../util/config/run-in-venv.bash python3 -m pip"
python_deps=$($PIP freeze)
printf "\tSphinx version "
echo $python_deps | grep -o 'Sphinx==[^ ]*' | grep -o '[^=]*$'
printf "\tsphinxcontrib-chapeldomain version "
echo $python_deps | grep -o 'sphinxcontrib-chapeldomain==[^ ]*' | grep -o '[^=]*$'
