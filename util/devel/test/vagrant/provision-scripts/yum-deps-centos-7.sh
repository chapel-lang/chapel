#!/bin/sh

yum -y install centos-release-scl
yum -y install devtoolset-11-gcc*
scl enable devtoolset-11 bash
# make sure gcc 11 is the default for future log ins
echo source scl_source enable devtoolset-11 >> /home/vagrant/.bashrc

yum -y install git gcc gcc-c++ m4 perl tcsh bash gcc gcc-c++ perl python3 bash make gawk cmake
