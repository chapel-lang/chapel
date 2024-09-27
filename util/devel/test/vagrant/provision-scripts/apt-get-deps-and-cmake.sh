#!/bin/sh

alias hide=eval

apt-get update
apt-get install -y gcc g++ m4 perl python3 python3-dev bash make mawk git pkg-config
apt-get install -y wget libssl-dev

hide MYTMP=`mktemp -d`
hide cd $MYTMP
wget https://github.com/Kitware/CMake/releases/download/v3.30.3/cmake-3.30.3.tar.gz
tar xvzf cmake-3.30.3.tar.gz
cd cmake-3.30.3
./bootstrap
make
make install

update-alternatives --install /usr/bin/cmake cmake /usr/local/bin/cmake 1
