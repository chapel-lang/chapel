#!/bin/bash

# Rebuilds the singularity image in each current subdirectory

for name in current/*
do
  if [ -f $name/singularity.def ]
  then
    echo "cd $name && singularity build --force --fakeroot singularity.sif singularity.def"
    ( cd $name && singularity build --force --fakeroot singularity.sif singularity.def )
  fi
done
