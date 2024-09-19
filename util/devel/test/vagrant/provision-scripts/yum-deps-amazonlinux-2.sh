#!/bin/sh

alias unsudo="sudo -u bin"
alias hide=eval

yum -y install git gcc gcc-c++ m4 perl python tcsh bash perl python python-devel python-setuptools bash make gawk python3 which

yum -y install wget tar openssl-devel

hide yum -y install sudo


hide MYTMP=`unsudo mktemp -d`
hide cd $MYTMP
unsudo wget https://github.com/Kitware/CMake/releases/download/v3.25.1/cmake-3.25.1.tar.gz
unsudo tar xvzf cmake-3.25.1.tar.gz
cd cmake-3.25.1
unsudo ./bootstrap
unsudo make
make install

update-alternatives --install /usr/bin/cmake cmake /usr/local/bin/cmake 1
