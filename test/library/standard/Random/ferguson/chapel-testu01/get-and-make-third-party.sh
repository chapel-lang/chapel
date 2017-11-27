#!/bin/bash

mkdir -p third-party
pushd third-party

if [ ! -f TestU01.zip ]
then
  wget http://www.iro.umontreal.ca/~simardr/testu01/TestU01.zip
fi

if [ ! -d TestU01-1.2.3 ]
then
  unzip TestU01.zip
fi

if [ ! -f install/lib/libtestu01.a ]
then
  pushd TestU01-1.2.3
  ./configure --prefix=`pwd`/../install
  make
  make install
  popd
fi

if [ ! -f pcg-c-basic-0.9.zip ]
then
  wget http://www.pcg-random.org/downloads/pcg-c-basic-0.9.zip
fi

if [ ! -d pcg-c-basic-0.9 ]
then
  unzip pcg-c-basic-0.9.zip
fi

pushd pcg-c-basic-0.9
popd

popd
