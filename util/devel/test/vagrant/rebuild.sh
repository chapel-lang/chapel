#!/bin/bash

# Rebuild all of the VMs. This will delete all data on the VMs.
# Don't do that if you have data you want to keep in any VM!

# enable resizing disks to get
#  config.vm.disk to work
#  (to resize the disk for some VMs that come too small)
export VAGRANT_EXPERIMENTAL="disks"

DIR=`pwd`

# Get new versions of Vagrant boxes
for name in current/*
do
  if [ -f $name/Vagrantfile ]
  then
    cd $name
    echo "------------ $name ---- vagrant box update"
    # update the box
    vagrant box update
    # destroy the current VM
    # (in case we need to make a new one with a new box)
    vagrant destroy -f
    # recreate the VM and install dependencies etc
    vagrant up
    # stop the VM
    vagrant halt
    cd "$DIR"
  fi
done

# Remove any old (now unused) vagrant boxes
vagrant box prune
