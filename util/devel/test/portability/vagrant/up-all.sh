#!/bin/bash

# Bring each VM up and leave it running.

set -exuo pipefail

for name in current/*
do
  if [ -f $name/Vagrantfile ]
  then
    cd $name
    echo "------------ $name ---- vagrant up"
    vagrant up
    cd ../..
  fi
done

