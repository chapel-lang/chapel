#!/usr/bin/env bash
# Active the virtual environment and run the command supplied
# usage: ./run-in-venv prog [args]

CWD=$(cd $(dirname $0) ; pwd)

# Perform checks and set up environment variables for virtual env.
source $CWD/run-in-venv-common.bash

exec "$1" "${@:2}"
