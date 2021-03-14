#!/bin/bash

# This script builds and locally installs a Chapel RPM module
#   * It is expected to run on a Cray system
#   * It is intended for use by Chapel developers
#   * It is intended to be simple enough to be manually adjusted / pasted

if [ ! -d "$CHPL_HOME" ]
then
  echo 'Please set $CHPL_HOME before running'
  exit -1
fi

URL=$1
BRANCH=$2
if [ -z "$URL" ] || [ -z "$BRANCH" ]
then
  echo "Usage: test_rpm_module github-url branch"
  echo "  For example, https://github.com/chapel-lang/chapel master"
  exit -1
fi

export CHPL_HOME_REPOSITORY="$URL"
export CHPL_GEN_RELEASE_BRANCH="$BRANCH"

echo "Creating and locally installing a Chapel RPM module"
echo " repository: $CHPL_HOME_REPOSITORY"
echo "     branch: $CHPL_GEN_RELEASE_BRANCH"
sleep 1

module swap PrgEnv-cray PrgEnv-gnu

cd $CHPL_HOME
source util/setchplenv.bash
START_DIR=`pwd`

if [ -f tar/chapel-developer.tar.gz ]
then
  echo "Removing old release tarball"
  rm tar/chapel-developer.tar.gz
fi

# Build a release tarball
# Version ('developer') needs to be specified for follow-on commands to be happy
./util/buildRelease/gen_release developer
# Generates tar/chapel-developer.tar.gz

cd "$START_DIR/util/build_configs/"

for r in ./*.rpm
do
  echo Removing old RPM $r
  rm $r
done

if [ -d chapel-developer ]
then
  echo Removing old installation directory
  rm -Rf ./chapel-developer
fi

# Build an RPM module
./cray-internal/chapel_build.bash -s setenv-example-3.bash -t ../../tar/chapel-developer.tar.gz -b developer -v
# Generates a chapel*.rpm

if [ -d "$START_DIR/rpm-install" ]
then
  echo "Removing old RPM test installation directory"
  rm -Rf "$START_DIR/rpm-install"
fi
mkdir -p "$START_DIR/rpm-install"
mkdir -p "$START_DIR/rpm-install/db"

echo "Installing RPM to $START_DIR/rpm-install"
rpm --initdb --dbpath "$START_DIR/rpm-install/db"
rpm --install --verbose --hash --oldpackage --nodeps --prefix "$START_DIR/rpm-install" --dbpath "$START_DIR/rpm-install/db" ./*.rpm

echo
echo
echo "Installed the test Chapel module to $START_DIR/rpm-install"
echo "Try these commands to load the module in a new shell:"
echo
echo "module swap PrgEnv-cray PrgEnv-gnu"
echo "export MODULEPATH=\$MODULEPATH:$START_DIR/rpm-install/modulefiles"
echo "module avail chapel"
echo "  # (then module load the version at the installed path)"
