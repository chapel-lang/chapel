#!/bin/bash

# Stop any running VMs

for name in current/*
do
  if [ -f $name/Vagrantfile ]
  then
    cd $name
    echo "------------ $name ---- vagrant halt"
    vagrant halt
    cd ..
  fi
done

