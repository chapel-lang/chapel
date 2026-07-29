#!/usr/bin/env bash
#This script will build a docker image using the Dockerfile and will execute homebrew install scripts inside the container
cd ${CHPL_HOME}/util/packaging/docker/test

# specify the platform as linux/amd64 to avoid warnings from aarch64 platforms

# Remove image with name homebrew_ci before creating a fresh image to avoid failures.
docker image rm --force homebrew_ci

# Build image
docker build . --load --platform linux/amd64 -t homebrew_ci
docker image inspect homebrew_ci

# Start the container and run a script to check homebrew install inside the running container
CONTAINER_NAME=homebrew-ci-test
docker container rm --force --volumes "$CONTAINER_NAME"
docker run --platform linux/amd64 -i --name "$CONTAINER_NAME" homebrew_ci /bin/bash < ${CHPL_HOME}/util/packaging/docker/test/brew_install.bash
CONTAINER_RUN=$?
if [ $CONTAINER_RUN -ne 0 ]
then
  echo "brew test bot commands failed inside ubuntu container"
  exit 1
  else
  echo "brew test bot commands succeeded inside ubuntu container"
fi
