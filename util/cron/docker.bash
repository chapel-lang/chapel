#!/bin/bash
# Common functions used for docker

check_docker_install(){
    local docker_arch
    docker_arch="$(docker info --format '{{.Architecture}}')"
    # check which architecture docker is running on and print the architecture
    if [[ $docker_arch == 'x86_64' ]]; then
        echo "x86_64 Architecture found"
    elif [[ $docker_arch == 'aarch64' ]]; then
        echo "aarch64 Architecture found"
    else
        echo "ERROR! Docker Desktop not installed:"
        echo "  * Install docker desktop from <https://docs.docker.com/docker-for-mac/install/>"
        exit 1
    fi
}

start_docker(){
    # Check if docker is running, if not start docker
    docker stats --no-stream > /dev/null 2>&1
    if [[ $? -ne 0 ]]; then
        echo "Docker is not running"
        if [[ "$OSTYPE" == "darwin"* ]]; then
            echo "Starting Docker Desktop on macOS"
            open -a Docker
        else
            echo "Attempting to start Docker on linux without sudo"
            systemctl start docker
        fi
    fi

    while (! docker stats --no-stream ); do
        # Docker takes a few seconds to initialize
        echo "Waiting for Docker to launch..."
        sleep 60
    done
    # wait to check docker install until docker is running or else it will always
    # get a non-zero length string back, which will cause the script to always
    # report x86 architecture...
    check_docker_install
}
