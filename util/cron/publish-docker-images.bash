#!/usr/bin/env bash

# This script will build the docker images for 'amd' and 'arm' platforms using buildx and publish the images to the docker registry

# Check if the script is run with correct arguement if not fail
required_vars=(docker_repository image_version username password)
for var in ${required_vars[@]}; do
    if [ -z "${!var}" ] ; then
        echo "${var} must be set."
        exit 1
    fi
done

echo "RepositoryName: $docker_repository"
echo "imageVersion: $image_version"

CWD=$(cd $(dirname $0) ; pwd)
source $CWD/common.bash
export CHPL_HOME=$(cd $CWD/../.. ; pwd)
log_info "Setting CHPL_HOME to: ${CHPL_HOME}"

# build_publish will build multi platform chapel docker images, tags them, and pushes the images to the docker repository .

build_publish(){

local registry="$1"
local imageName="$2"
local version="$3"

# the below buildx command will build images for amd and arm, tags with the tags specified, and pushes it to the docker repository($registry)
docker buildx build --platform=linux/amd64,linux/arm64 . --push -t $registry/$imageName:$version -t  $registry/$imageName:latest

if [ $? -ne 0 ]
then
      echo "docker publish using buildx failed "
      exit 1
else
      echo "docker publish using buildx succeeded "
fi

}

# Get the repository name and chapel version, Build chapel docker images and push to docker hub repository .
#build and publish chapel docker image
docker login -u $username -p $password
if [ $? -ne 0 ]
then
      echo " Docker login failed "
      exit 1
else
      echo "docker login succeeded "
fi

cd $CHPL_HOME
build_publish $docker_repository  chapel $image_version

docker login -u $username -p $password
if [ $? -ne 0 ]
then
      echo " Docker login failed "
      exit 1
else
      echo "docker login succeeded "
fi
#build and publish chapel-gasnet docker image
cd $CHPL_HOME/util/packaging/docker/gasnet
build_publish $docker_repository  chapel-gasnet $image_version

docker login -u $username -p $password
if [ $? -ne 0 ]
then
      echo " Docker login failed "
      exit 1
else
      echo "docker login succeeded "
fi
#build and publish chapel-gasnet-smp docker image
cd $CHPL_HOME/util/packaging/docker/gasnet-smp
build_publish $docker_repository  chapel-gasnet-smp $image_version
