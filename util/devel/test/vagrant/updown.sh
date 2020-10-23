#!/bin/bash

# Bring each VM up and then shut it down.
# Useful for making sure they are basically working.

for name in current/*
do
  if [ -f $name/Vagrantfile ]
  then
    cd $name
    echo "------------ $name ---- vagrant up"
    vagrant up
    vagrant halt
    cd ..
  fi
done

