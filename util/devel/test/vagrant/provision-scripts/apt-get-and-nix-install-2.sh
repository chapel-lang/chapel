#!/bin/bash

# Note, this is not run all as root, so use sudo where needed

# gather the build dependencies environment
mkdir chpl-nix-deps
cp chpl-nix-deps.nix chpl-nix-deps/flake.nix
cd chpl-nix-deps
bash -c 'nix print-dev-env --accept-flake-config < /dev/null > activate.sh'

# add code to append includes to compiler command
# these includes are necessary to avoid problems not finding sys/types.h
# which is actually defined by clang itself.
# TODO: is there a better way to handle this on nix?
echo 'export CHPL_TARGET_CC="$CHPL_TARGET_CC $CHPL_CLANG_INCLUDES"' >> activate.sh
echo 'export CHPL_TARGET_CXX="$CHPL_TARGET_CXX $CHPL_CLANG_INCLUDES"' >> activate.sh

# make it available to future logins
echo 'source chpl-nix-deps/activate.sh' >> /home/vagrant/.bash_profile
