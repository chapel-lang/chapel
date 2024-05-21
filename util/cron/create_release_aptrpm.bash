#!/usr/bin/env bash
#
# Create and store a release apt/rpm.
#
# Argument are set via the environment:
#   CHPL_VERSION: The version of the release to create.
#   CHPL_TARBALL: The location of the tarball to use. If not set, the tarball will be downloaded.
#   OUTPUT_DIR: The directory to to copy the built packages to. If not set, the tarball will be left in the default location.
#   PACKAGE_TYPE: The type of package to build. Either 'apt' or 'rpm'.
#   OS: The OS to build the package for. e.g. 'ubuntu22'
#   PACKAGE_NAME: The name of the package to build. e.g. 'chapel'
#   PACKAGE_VERSION: The version of the package to build, usually this is '1'
#   DOCKER_DIR_NAME: The name of the directory in util/packaging/{PACKAGE_TYPE} that contains the Dockerfile to use.
#   PARALLEL: The number of cores to use for building the package. Default is 1.

#
# Arguments
#
if [ -z "$CHPL_VERSION" ]; then echo "CHPL_VERSION must be set."; exit 1; fi
if [ -z "$PACKAGE_TYPE" ]; then echo "PACKAGE_TYPE must be set."; exit 1; fi
if [ -z "$OS" ]; then echo "OS must be set."; exit 1; fi
if [ -z "$PACKAGE_NAME" ]; then echo "PACKAGE_NAME must be set."; exit 1; fi
if [ -z "$PACKAGE_VERSION" ]; then echo "PACKAGE_VERSION must be set."; exit 1; fi
if [ -z "$DOCKER_DIR_NAME" ]; then echo "DOCKER_DIR_NAME must be set."; exit 1; fi
PARALLEL=${PARALLEL:-1}


CWD=$(cd $(dirname $0) ; pwd)
source $CWD/common.bash
source $CWD/functions.bash

export CHPL_HOME=$(cd $CWD/../.. ; pwd)
log_info "Setting CHPL_HOME to: ${CHPL_HOME}"

source $CHPL_HOME/util/packaging/common/build_helpers.sh

# if using a local tarball, copy it to the expected location
if [ -n "$CHPL_TARBALL" ]; then
  log_info "Using local tarball: $CHPL_TARBALL"
  cp $CHPL_TARBALL $CHPL_HOME/util/packaging/common/chapel-${CHPL_VERSION}.tar.gz
fi

log_info "Building $PACKAGE_NAME $PACKAGE_TYPE package on $OS"
__build_packages $PACKAGE_TYPE $OS $PACKAGE_NAME $CHPL_VERSION $PACKAGE_VERSION $DOCKER_DIR_NAME $PARALLEL

log_info "Getting $PACKAGE_NAME $PACKAGE_TYPE package from docker image"
__get_packages $PACKAGE_TYPE $OS $PACKAGE_NAME $CHPL_VERSION $PACKAGE_VERSION

if [ -n "$OUTPUT_DIR" ]; then
  log_info "Copying packages to $OUTPUT_DIR"
  mkdir -p $OUTPUT_DIR
  cp -r $CHPL_HOME/util/packaging/$PACKAGE_TYPE/build/* $OUTPUT_DIR
fi
