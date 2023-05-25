#!/usr/bin/env bash

# script for singularity image 'run' command

if [ -d chapel ]
then
  echo chapel directory already exists - updating
  git checkout main
  git pull
else
  echo cloning chapel
  git clone https://github.com/chapel-lang/chapel
fi

cd chapel
source util/quickstart/setchplenv.bash
make -j4
make check
