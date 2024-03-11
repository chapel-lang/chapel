#!/bin/sh

alias unsudo="sudo -u bin"
alias hide=eval

yum -y install centos-release-scl
yum -y install devtoolset-11-gcc*
yum -y install epel-release

scl enable devtoolset-11 bash
# make sure gcc 11 is the default for future log ins
echo source scl_source enable devtoolset-11 >> /home/vagrant/.bashrc

yum -y install git gcc gcc-c++ m4 perl tcsh bash python3 make gawk wget openssl-devel

# cmake3 package is available but it is cmake 3.17 for CentOS 7
# a newer cmake is required.

hide MYTMP=`unsudo mktemp -d`
hide cd $MYTMP
unsudo wget https://github.com/Kitware/CMake/releases/download/v3.25.1/cmake-3.25.1.tar.gz
unsudo tar xvzf cmake-3.25.1.tar.gz
cd cmake-3.25.1
unsudo ./bootstrap
unsudo make
make install

update-alternatives --install /usr/bin/cmake cmake /usr/local/bin/cmake 1
