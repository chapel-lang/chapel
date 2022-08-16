#!/bin/bash

# Deletes the singularity image in each current subdirectory

for name in current/*
do
  if [ -f $name/singularity.def ]
  then
    echo "cd $name && rm -Rf singularity.sif"
    ( cd $name && rm -Rf singularity.sif )
  fi
done
