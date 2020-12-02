#!/bin/bash

# Destroy (delete) each VM
# Removes space occupied by each VM
# Don't do this if you have work you want to save in the VMs!

for name in current/*
do
  if [ -f $name/Vagrantfile ]
  then
    cd $name
    echo "------------ $name ---- vagrant destroy"
    vagrant destroy -f
    cd ..
  fi
done

