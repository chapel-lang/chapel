#!/bin/bash

# Destroy (delete) each VM
# Removes space occupied by each VM
# Don't do this if you have work you want to save in the VMs!

if [ -z "$VM_NAME" ]
then
  echo "Please set the VM_NAME environment variable to the name of the VM you want to run this on."
  exit 1
fi

for name in current/*
do
  if [ -f $name/Vagrantfile ]
  then
    if [ "$VM_NAME" != "all" ]
    then
      if [ "$name" != "current/$VM_NAME" ]
      then
        continue
      fi
    fi
    cd $name
    echo "------------ $name ---- vagrant destroy"
    vagrant destroy -f
    cd ../..
  fi
done

