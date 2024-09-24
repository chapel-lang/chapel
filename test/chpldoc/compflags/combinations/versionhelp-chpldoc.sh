#!/usr/bin/env bash
CWD=$(cd $(dirname $0) ; pwd)
compiler=$3

echo -n `basename $compiler`
cat $CWD/../../../compflags/bradc/printstuff/version.goodstart
# During pre-release mode
diff $CWD/../../../../compiler/main/BUILD_VERSION $CWD/zero.txt > /dev/null 2>&1 && echo "" || \
  { echo -n " pre-release (" && cat $CWD/../../../../compiler/main/BUILD_VERSION | tr -d \"\\n && echo ")" ; }
# During release mode:
#echo ""

# print Sphinx and chapeldomain versions
python=$($CWD/../../../../util/config/find-python.sh)
chpldeps=$("$python" "$CWD/../../../../util/chplenv/chpl_home_utils.py" --chpldeps)
printf "\tSphinx version "
ls $chpldeps | grep -io 'Sphinx-[0-9.]*.dist-info' | \
  grep -o '[0-9.]*' | rev | cut -c2- | rev
printf "\tsphinxcontrib-chapeldomain version "
ls $chpldeps | grep -io 'sphinxcontrib_chapeldomain-[0-9.]*.dist-info' | \
  grep -o '[0-9.]*' | rev | cut -c2- | rev
