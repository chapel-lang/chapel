#!/usr/bin/env bash

# This script will build the docker images for 'amd' and 'arm' platforms using buildx and publish the images to the docker registry

# Check if the script is run with correct arguement if not fail
if [  $# -ne 2  ]
then
    echo " Docker repository and/or image-version not supplied "
    echo " run with ./publish-docker-images.bash doc_repository image_version "
    exit 1
fi
echo "RepositoryName: $1"
echo "imageVersion: $2"

CWD=$(cd $(dirname $0) ; pwd)
source $CWD/common.bash
source $CWD/docker.bash
export CHPL_HOME=$(cd $CWD/../.. ; pwd)
log_info "Setting CHPL_HOME to: ${CHPL_HOME}"

start_docker
 
# build_publish will build multi platform chapel docker images, tags them, and pushes the images to the docker repository .

build_publish(){

local registry="$1"
local imageName="$2" 
local version="$3"

# the below buildx command will build images for amd and arm, tags with the tags specified, and pushes it to the docker repository($registry)
docker buildx build --platform=linux/amd64,linux/arm64 . --push -t $registry/$imageName:$version -t  $registry/$imageName:latest

if [ $PUBLISHED -ne 0 ]
then
      echo "docker publish using buildx failed " 
      exit 1
else
      echo "docker publish using buildx succeeded "
fi   

}

# Get the repository name and chapel version, Build chapel docker images and push to docker hub repository . 
#build and publish chapel docker image
cd $CHPL_HOME
build_publish $1  chapel $2
#build and publish chapel-gasnet docker image
cd $CHPL_HOME/util/packaging/docker/gasnet
build_publish $1  chapel-gasnet $2
#build and publish chapel-gasnet-smp docker image
cd $CHPL_HOME/util/packaging/docker/gasnet-smp
build_publish $1  chapel-gasnet-smp $2