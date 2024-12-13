#!/usr/bin/env bash
#
# This script will build, (sanity) test, and push the chapel, chapel-gasnet, and
# chapel-gasnet-smp Docker images. Uses the 'nightly' tag by default, or
# 'latest' and a release version tag if specified.
#
# Assumes Docker is already running on the system, logged into an account with
# appropriate permissions to push the images.
#
# Expected environment variables:
# - RELEASE_VERSION (optional): If set, will also push the image tagged as
#   'latest' and this version. Should match version in release branch name.


UTIL_CRON_DIR=$(cd $(dirname ${BASH_SOURCE[0]}) ; pwd)
source $UTIL_CRON_DIR/common.bash
export CHPL_HOME=$(cd $UTIL_CRON_DIR/../.. ; pwd)
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

  patch $patch_args ./Dockerfile << EOF
$nightlypatch
EOF
}

# Build, test, and push a Docker image.
# Args:
# - image name without tag
# - test script location
# - release version tag to use (optional, builds nightly otherwise)
update_image() {
  local baseImageName="$1"
  local script="$2"
  local release_tag="$3"

  # Use specified release version tag, or 'nightly' if not specified
  local imageName="${baseImageName}:${release_tag:-nightly}"

  log_info "Starting $imageName..."

  # Remove any existing image with the tag before building
  docker image rm --force "$imageName"

  # Build and push image
  # Note: We push before testing due to a limitation of Docker
  # (https://github.com/docker/buildx/issues/59) which prevents loading a
  # multi-arch image without pushing. This means we may push a broken nightly
  # image before erroring out; it's important that release pushes come after
  # all nightly pushes so we can't push a broken release image.
  # Anna, 2024-10-07
  docker buildx build --platform=linux/amd64,linux/arm64 --push . -t "$imageName"
  BUILD_RESULT=$?
  # Also push as 'latest' tag if this is a release build
  if [ -n "$release_tag" ]
  then
    # Use base image name (without tag) to use Docker's default tag 'latest'
    docker buildx build --platform=linux/amd64,linux/arm64 --push . -t "$baseImageName"
  fi

  if [ $BUILD_RESULT -ne 0 ]
  then
        echo "docker build failed for $imageName image"
        exit 1
  fi

  # Run test script inside container
  echo 'writeln("Hello, world!");' > hello.chpl
  docker run --rm -i "$imageName"  <  "$script"
  CONTAINER_RUN=$?
  # Clean up scratch chpl file for testing
  rm hello.chpl

  if [ $CONTAINER_RUN -ne 0 ]
  then
        echo "docker commands failed inside chapel $imageName container"
        exit 1
  else
        echo "docker commands succeeded inside chapel $imageName container"
  fi

  log_info "Completed $imageName"
}

# Build, test, and push all Chapel Docker images.
# Args:
# - release version tag to use (optional, builds nightly otherwise)
update_all_images() {
  local release_tag="$1"

  cd "$CHPL_HOME"
  update_image chapel/chapel "${CHPL_HOME}/util/cron/docker-chapel.bash" "$release_tag"

  cd "$CHPL_HOME/util/packaging/docker/gasnet"
  dockerfile_nightly_patch
  update_image chapel/chapel-gasnet "${CHPL_HOME}/util/cron/docker-gasnet.bash" "$release_tag"
  # Clean up after patch changes
  dockerfile_nightly_patch -R

  cd "$CHPL_HOME/util/packaging/docker/gasnet-smp"
  dockerfile_nightly_patch
  update_image chapel/chapel-gasnet-smp "${CHPL_HOME}/util/cron/docker-gasnet.bash" "$release_tag"
  # Clean up after patch changes
  dockerfile_nightly_patch -R
}
# END FUNCTIONS


if [ -n "$RELEASE_VERSION" ]
then
  log_info "Building and pushing nightly and release-tagged images for version: $RELEASE_VERSION"
  release_branch="release/$RELEASE_VERSION"
  if [ "$(git rev-parse --abbrev-ref HEAD)" != "$release_branch" ]
  then
    log_error "Not on expected release branch $release_branch for version $RELEASE_VERSION, aborting"
    exit 1
  fi
else
  log_info "Building and pushing nightly images"
fi

# Build and push nightly images
update_all_images

# Build and push release-tagged images, if RELEASE_VERSION was specified.
# Runs after all nightly images, to abort if any fail.
if [ -n "$RELEASE_VERSION" ]
then
  update_all_images "$RELEASE_VERSION"
fi
