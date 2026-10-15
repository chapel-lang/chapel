#!/bin/bash

# Bring each VM up and leave it running.

set -exuo pipefail

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
    echo "------------ $name ---- vagrant up"
    vagrant up
    cd ../..
  fi
done

