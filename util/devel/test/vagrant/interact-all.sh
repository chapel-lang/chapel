#!/bin/bash

# Log in to each VM in turn to run commands manually
# Assumes VMs are up and leaves them running

for name in current/*
do
  if [ -f $name/Vagrantfile ]
  then
    cd $name
    echo "------------ $name ---- vagrant ssh"
    #vagrant up
    vagrant ssh
    #vagrant halt
    cd ..
  fi
done

