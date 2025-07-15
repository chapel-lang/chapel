#!/usr/bin/env bash
#
# Create and store a release apt/rpm.
#
# Argument are set via the environment:
#   CHPL_VERSION: The version of the release to create.
#   CHPL_TARBALL: The location of the tarball to use. If not set, the tarball will be downloaded.
#   OS: The OS to build the package for. e.g. 'ubuntu22'
#   PACKAGE_NAME: The name of the package to build. e.g. 'chapel'
#   PACKAGE_VERSION: The version of the package to build, usually this is '1'
#   PARALLEL: The number of cores to use for building the package. Default is 1.

#
# Arguments
#
if [ -z "$CHPL_VERSION" ]; then echo "CHPL_VERSION must be set."; exit 1; fi
if [ -z "$OS" ]; then echo "OS must be set."; exit 1; fi
if [ -z "$PACKAGE_NAME" ]; then echo "PACKAGE_NAME must be set."; exit 1; fi
if [ -z "$PACKAGE_VERSION" ]; then echo "PACKAGE_VERSION must be set."; exit 1; fi
PARALLEL=${PARALLEL:-1}


UTIL_CRON_DIR=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)
source $UTIL_CRON_DIR/common.bash
source $UTIL_CRON_DIR/functions.bash

export CHPL_HOME=$(cd $UTIL_CRON_DIR/../.. ; pwd)
log_info "Setting CHPL_HOME to: ${CHPL_HOME}"

# if any errors occur, this script should fail
set -e

source $CHPL_HOME/util/packaging/common/build_helpers.sh

# if using a remote tarball, download it
if [ -n "$CHPL_TARBALL_LOCATION" ]; then
  log_info "Downloading Chapel tarball from '$CHPL_TARBALL_LOCATION'"
  mkdir -p $CHPL_HOME/util/packaging/tarballs
  wget --no-check-certificate ${CHPL_TARBALL_LOCATION} -O $CHPL_HOME/util/packaging/tarballs/chapel-${CHPL_VERSION}.tar.gz
fi

# if using a local tarball, copy it to the expected location
if [ -n "$CHPL_TARBALL" ]; then
  log_info "Using local tarball: $CHPL_TARBALL"
  mkdir -p $CHPL_HOME/util/packaging/tarballs
  cp $CHPL_TARBALL $CHPL_HOME/util/packaging/tarballs/chapel-${CHPL_VERSION}.tar.gz
fi

# For debian builds, setup some extra proxy/network information
if [ $(__package_type_from_os $OS) == "apt" ]; then
  apt_conf="./util/packaging/common/apt.conf"
  rm -f $apt_conf

  if [ -z "$PROXY_ADDRESS_FOR_HTTP" ]; then echo "PROXY_ADDRESS_FOR_HTTP must be set."; exit 1; fi
  if [ -z "$PROXY_ADDRESS_FOR_HTTPS" ]; then echo "PROXY_ADDRESS_FOR_HTTPS must be set."; exit 1; fi

  echo "Acquire::http::proxy \"$PROXY_ADDRESS_FOR_HTTP\";" >> $apt_conf
  echo "Acquire::https::proxy \"$PROXY_ADDRESS_FOR_HTTPS\";" >> $apt_conf
  export INJECT_BEFORE_DEPS="COPY ./common/apt.conf /etc/apt/apt.conf"
fi

# if BUILD_CROSS_PLATFORM is set, build the cross-platform package
if [ -n "$BUILD_CROSS_PLATFORM" ]; then
  log_info "Building cross-platform $PACKAGE_NAME $PACKAGE_TYPE package on $OS"
  __build_all_packages $OS $PACKAGE_NAME $CHPL_VERSION $PACKAGE_VERSION $PARALLEL
else
  log_info "Building $PACKAGE_NAME $PACKAGE_TYPE package on $OS"
  __build_native_package $OS $PACKAGE_NAME $CHPL_VERSION $PACKAGE_VERSION $PARALLEL
fi

log_info "Testing $PACKAGE_NAME"
__test_all_packages
