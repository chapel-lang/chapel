#!/bin/bash

# Deletes the chapel checkout in each image's directory

for name in current/*
do
  if [ -f $name/apptainer.def ]
  then
    echo "cd $name && rm -Rf chapel"
    ( cd $name && rm -Rf chapel )
  fi
done
