#!/bin/bash

# Git cleans the chapel checkout in each image's directory

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

    cd $name
    cd chapel
    git clean -ffdx
  fi
done
