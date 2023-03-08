#!/bin/bash

# Rebuilds the apptainer image in each current subdirectory

for name in current/*
do
  if [ -f $name/apptainer.def ]
  then
    echo "cd $name && apptainer build --force --fakeroot apptainer.sif apptainer.def"
    ( cd $name && apptainer build --force --fakeroot apptainer.sif apptainer.def )
  fi
done
