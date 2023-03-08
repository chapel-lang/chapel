#!/bin/bash

# Deletes the apptainer image in each current subdirectory

for name in current/*
do
  if [ -f $name/apptainer.def ]
  then
    echo "cd $name && rm -Rf apptainer.sif"
    ( cd $name && rm -Rf apptainer.sif )
  fi
done
