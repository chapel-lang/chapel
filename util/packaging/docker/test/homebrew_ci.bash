#!/usr/bin/env bash
#This script will build a docker image using the Dockerfile and will execute home-brew install scripts inside the container
CWD=$(cd $(dirname $0) ; pwd)
export CHPL_HOME=$(cd $CWD/../.. ; pwd)

cd ${CHPL_HOME}/packaging/docker/test

# Remove image with name homebrew_ci before creating a fresh image to avoid failures.
docker image rm --force homebrew_ci
docker build . --load -t homebrew_ci
containerid= docker image ls | grep 'homebrew_ci' | awk '{print$3}'

# Start the container and run a script to check homebrew install inside the running container
docker run --rm -i homebrew_ci /bin/bash < ${CHPL_HOME}/packaging/docker/test/brew_install.bash
CONTAINER_RUN=$?
    if [ $CONTAINER_RUN -ne 0 ]
    then
      echo "brew test bot commands failed inside ubuntu container"
      exit 1
      else
      echo "brew test bot commands succeeded inside ubuntu container"
    fi
