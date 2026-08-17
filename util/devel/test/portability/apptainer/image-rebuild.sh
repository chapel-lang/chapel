#!/bin/bash

# Rebuilds the apptainer image in each current subdirectory

if [ -z "$APPTAINER_IMAGE" ]
then
  echo "Please set the APPTAINER_IMAGE environment variable to the name of the config you want to run this on."
  exit 1
fi

for name in current/*
do
  if [ -f $name/image.def ]
  then
    if [ "$APPTAINER_IMAGE" != "all" ]
    then
      if [ "$name" != "current/$APPTAINER_IMAGE" ]
      then
        continue
      fi
    fi

    echo "cd $name && apptainer build --force --fakeroot image.sif image.def"
    ( cd $name && apptainer build --force --fakeroot image.sif image.def )
  fi
done
