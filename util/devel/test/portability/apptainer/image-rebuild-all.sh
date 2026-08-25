#!/bin/bash

# Rebuilds the apptainer image in each current subdirectory

set -exuo pipefail

for name in current/*
do
  if [ -f $name/image.def ]
  then
    echo "cd $name && apptainer build --force --fakeroot image.sif image.def"
    ( cd $name && apptainer build --force --fakeroot image.sif image.def )
  fi
done
