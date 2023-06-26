#!/bin/bash

# Note, this is not run all as root, so use sudo where needed

sudo apt-get update

sh <(curl -L https://nixos.org/nix/install) --daemon --yes

sudo bash -c "echo 'experimental-features = nix-command flakes' >> /etc/nix/nix.conf"
