#!/usr/bin/env bash
#
# This script will build, (sanity) test, and push the chapel, chapel-gasnet, and
# chapel-gasnet-smp Docker images. Uses the 'nightly' tag by default, or
# 'latest' and a release version tag if specified.
#
# Assumes Docker is already running on the system, logged into an account with
# appropriate permissions to push the images.

CWD=$(cd $(dirname $0) ; pwd)
source $CWD/common.bash
export CHPL_HOME=$(cd $CWD/../.. ; pwd)
log_info "Setting CHPL_HOME to: ${CHPL_HOME}"
export CHPL_NIGHTLY_TEST_CONFIG_NAME="docker"

# BEGIN FUNCTIONS

# Patch the Dockerfile to build FROM the nightly image instead of latest.
# Assumes the Dockerfile is available at ./Dockerfile.
# Arguments are forwarded to `patch` command.
dockerfile_nightly_patch() {
  local patch_args="$*"

  local nightlypatch="
1c1
< FROM chapel/chapel:latest
---
> FROM chapel/chapel:nightly
"

  patch "$patch_args" ./Dockerfile << EOF
$nightlypatch
EOF
}

# Build, test, and push a Docker image.
# Args:
# - image name without tag
# - test script location
# - release version tag to use (optional, just 'nightly' otherwise)
update_image() {
  local baseImageName="$1"
  local script="$2"
  local release_tag="$3"

  # Use specified release version tag, or 'nightly' if not specified
  local imageName="${baseImageName}:${release_tag:-nightly}"

  # Remove any existing image with the tag before building nightly docker image
  if [ -n "$release_tag" ]
  then
    docker image rm --force "$baseImageName"
  fi

  # Build image
  docker buildx build --platform=linux/amd64,linux/arm64 . -t "$imageName"
  BUILD_RESULT=$?
  if [ $BUILD_RESULT -ne 0 ]
  then
        echo "docker build failed for $imageName image"
        exit 1
  fi

  # Set up to test container
  cd "${CHPL_HOME}/util/cron"
  echo 'writeln("Hello, world!");' > hello.chpl

  # Run test script inside container
  docker run --rm -i "$imageName"  <  "$script"
  CONTAINER_RUN=$?

  # Clean up after our scratch test script, whether it succeeded or not
  rm hello.chpl

  if [ $CONTAINER_RUN -ne 0 ]
  then
        echo "docker commands failed inside chapel $imageName container"
        exit 1
  else
        echo "docker commands succeeded inside chapel $imageName container"
  fi

  # Push image after testing has succeeded
  docker buildx build --platform=linux/amd64,linux/arm64 . --push -t "$imageName"
  # Also push as 'latest' tag if this is a release build
  if [ -n "$release_tag" ]
  then
    # Use base image name (without tag) to use Docker's default tag 'latest'
    docker buildx build --platform=linux/amd64,linux/arm64 . --push -t "$baseImageName"
  fi
}

# Build, test, and push all Chapel Docker images.
# Args:
# - release version tag to use (optional, just 'nightly' otherwise)
update_all_images() {
  local release_tag="$1"

  cd "$CHPL_HOME"
  update_image chapel/chapel "${CHPL_HOME}/util/cron/docker-chapel.bash" "$release_tag"

  cd "$CHPL_HOME/util/packaging/docker/gasnet"
  dockerfile_nightly_patch
  update_image chapel/chapel-gasnet "${CHPL_HOME}/util/cron/docker-gasnet.bash" "$release_tag"
  dockerfile_nightly_patch -R

  cd "$CHPL_HOME/util/packaging/docker/gasnet-smp"
  dockerfile_nightly_patch
  update_image chapel/chapel-gasnet-smp "${CHPL_HOME}/util/cron/docker-gasnet.bash" "$release_tag"
  dockerfile_nightly_patch -R
}
# END FUNCTIONS


# Build and push nightly images
update_all_images

# Build and push release images after ALL nightly builds have succeeded, if
# release tag was specified.
if [ -n "$RELEASE_VERSION" ]
then
  update_all_images "$RELEASE_VERSION"
fi
