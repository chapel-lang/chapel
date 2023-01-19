#!/bin/bash
# Common functions used for docker 

check_docker_install(){
     # check if docker desktop is installed in mac if not install it before running the tests.
    if [[ -n "$(docker info --format '{{.Architecture}}' | grep 'aarch64')" ]]; then
        echo "aarch64 Architecture found. Skipping installation ..." 
    else
        echo "ERROR! Docker Desktop not installed:"
        echo "  * Install docker desktop from <https://docs.docker.com/docker-for-mac/install/>"
        exit 1
    fi

}

start_docker(){
    check_docker_install
    # Start docker desktop and wait for it to respond
    printf "Starting Docker for Mac";
    colima start;

    while (! docker stats --no-stream ); do
  # Docker takes a few seconds to initialize
        echo "Waiting for Docker to launch..."
        sleep 60
    done
}


