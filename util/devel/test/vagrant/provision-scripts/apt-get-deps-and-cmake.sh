#!/bin/sh

alias hide=eval

apt-get update
apt-get install -y gcc g++ m4 perl python3 python3-dev bash make mawk git pkg-config

apt-get install -y wget libssl-dev
hide CMAKE_VERSION=3.30.3
hide MYTMP=`mktemp -d`
hide cd $MYTMP
wget https://github.com/Kitware/CMake/releases/download/v$CMAKE_VERSION/cmake-$CMAKE_VERSION.tar.gz
tar xvzf cmake-$CMAKE_VERSION.tar.gz
cd cmake-$CMAKE_VERSION
./bootstrap
make $MAKEJ
make install

update-alternatives --install /usr/bin/cmake cmake /usr/local/bin/cmake 1
