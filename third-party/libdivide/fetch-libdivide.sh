#!/bin/sh

if [ -d libdivide ]
then
  echo libdivide already fetched!
else 
  echo Fetching libdivide
  git clone https://github.com/ridiculousfish/libdivide.git
fi
