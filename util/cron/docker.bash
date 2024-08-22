#!/bin/bash
# Common functions used for docker

check_docker_install(){
    local docker_version
    $docker_version="$(docker info --format '{{.Architecture}}')"
    # check which architecture docker is running on and print the architecture
    if [[ -n "$docker_version | grep 'x86')" ]]; then
        echo "x86 Architecture found"
    elif [[ -n "$docker_version | grep 'aarch64')" ]]; then
        echo "aarch64 Architecture found"
    else
        echo "ERROR! Docker Desktop not installed:"
        echo "  * Install docker desktop from <https://docs.docker.com/docker-for-mac/install/>"
    fi
}

start_docker(){
    check_docker_install
    # Start docker desktop and wait for it to respond
    echo "Starting Docker";

    while (! docker stats --no-stream ); do
  # Docker takes a few seconds to initialize
        echo "Waiting for Docker to launch..."
        sleep 60
    done
}
