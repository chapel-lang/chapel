#!/bin/bash

# Rebuild all of the VMs. This will delete all data on the VMs.
# Don't do that if you have data you want to keep in any VM!

# Get new versions of Vagrant boxes
./boxupdate.sh
# Destroy existing VMs
./destroy.sh
# Remove old (now unused) vagrant boxes
vagrant box prune
