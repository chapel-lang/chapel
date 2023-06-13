#!/bin/bash

# Note, this is not run all as root, so use sudo where needed

# gather the build dependencies environment
mkdir chpl-nix-deps
cp chpl-nix-deps.nix chpl-nix-deps/flake.nix
cd chpl-nix-deps
bash -c 'nix print-dev-env --accept-flake-config < /dev/null >> activate.sh'

# make it available to future logins
echo 'source chpl-nix-deps/activate.sh' >> /home/vagrant/.bash_profile
