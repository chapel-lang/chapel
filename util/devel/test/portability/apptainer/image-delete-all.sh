#!/bin/bash

# Deletes the apptainer image in each current subdirectory

set -exuo pipefail

for name in current/*
do
  if [ -f $name/image.def ]
  then
    echo "cd $name && rm -Rf image.sif"
    ( cd $name && rm -Rf image.sif )
  fi
done
