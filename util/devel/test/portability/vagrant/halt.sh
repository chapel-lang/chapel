#!/bin/bash

# Stop a running VM

if [ -z "$VM_NAME" ]
then
  echo "Please set the VM_NAME environment variable to the name of the VM you want to run this on."
  exit 1
fi

DIR=`pwd`

for name in current/*
do
  if [ -f $name/Vagrantfile ]
  then
    # skip if this isn't a desired VM to run on
    if [ "$VM_NAME" != "all" ]
    then
      if [ "$name" != "current/$VM_NAME" ]
      then
        continue
      fi
    fi
    cd $name
    echo "------------ $name ---- vagrant halt"
    vagrant halt
    cd "$DIR"
  fi
done
