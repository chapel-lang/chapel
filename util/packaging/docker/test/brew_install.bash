#!/usr/bin/env bash

# Script used in docker exec command to run homebrew install inside the container
brew install --build-from-source chapel.rb
INSTALL_STATUS=$?
    if [ $INSTALL_STATUS -ne 0 ]
    then
      echo "brew install --build-from-source chapel.rb failed" 
      exit 1
      else
      echo "brew install --build-from-source chapel.rb succeeded"
    fi
chpl --version
CHPL_INSTALL=$?
    if [ $CHPL_INSTALL -ne 0 ]
    then
      echo "chpl --version failed" 
      exit 1
    else
      echo "chpl --version succeeded"
    fi