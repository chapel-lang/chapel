#!/bin/bash

# Downloads new vagrant boxes, but doesn't take any
# action to get the VMs to use them.

for name in current/*
do
  if [ -f $name/Vagrantfile ]
  then
    cd $name
    echo "------------ $name ---- vagrant box update"
    vagrant box update
    cd ..
  fi
done

