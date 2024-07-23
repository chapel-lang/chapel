#!/usr/bin/env bash
#
# This script will build, run, and verify chapel, gasnet, and gasnet-smp docker images.

CWD=$(cd $(dirname $0) ; pwd)
source $CWD/common.bash
source $CWD/docker.bash
export CHPL_HOME=$(cd $CWD/../.. ; pwd)
log_info "Setting CHPL_HOME to: ${CHPL_HOME}"

start_docker

# build_image function takes image name and docker script location as arguments.
# Builds the image with the name from arg$1, runs the container and execute the install and verify script located in the location $2.
build_image() {
  local imageName="$1"
  local script="$2"
  # Remove any existing image with the tag before building docker image
  docker image rm --force $imageName
  docker build --push . -t $imageName
  containerid= docker image ls | grep $imageName | awk '{print$3}'
  cd ${CHPL_HOME}/util/cron
  echo 'writeln("Hello, world!");' > hello.chpl

  docker run --rm -i $imageName  <  $script

  CONTAINER_RUN=$?
  if [ $CONTAINER_RUN -ne 0 ]
  then
        echo "docker commands failed inside chapel $imageName container"
        exit 1
  else
        echo "docker commands succeeded inside chapel $imageName container"
  fi
}

# Patch the Dockerfile to build FROM the nightly image instead of latest
dockerfile_nightly_patch() {
  local nightlypatch="
1c1
< FROM chapel/chapel:latest
---
> FROM chapel/chapel:nightly
"
  patch Dockerfile << EOF
$nightlypatch
EOF
}

# Build chapel Docker images
cd $CHPL_HOME
build_image chapel/chapel:nightly  ${CHPL_HOME}/util/cron/docker-chapel.bash

cd $CHPL_HOME/util/packaging/docker/gasnet
dockerfile_nightly_patch
build_image chapel/chapel_gasnet:nightly ${CHPL_HOME}/util/cron/docker-gasnet.bash

cd $CHPL_HOME/util/packaging/docker/gasnet-smp
dockerfile_nightly_patch
build_image chapel/chapel_gasnet_smp:nightly ${CHPL_HOME}/util/cron/docker-gasnet.bash

export CHPL_NIGHTLY_TEST_CONFIG_NAME="docker"
