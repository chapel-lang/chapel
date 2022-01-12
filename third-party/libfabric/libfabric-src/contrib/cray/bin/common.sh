#!/bin/bash

# DIR is expected to be defined by files that include these common variables
BUILD_VERSION=$(cat $DIR/../share/version)
ROOT_INSTALL_PATH=$LIBFABRIC_BUILD_PATH/virtualenv/
PREREQ_INSTALL_PATH=$ROOT_INSTALL_PATH/prerequisites
VIRTUALENV_INSTALL_PATH=$ROOT_INSTALL_PATH/venv-${BUILD_VERSION}

