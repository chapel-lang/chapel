#!/bin/sh

yum -y install git gcc gcc-c++ m4 perl python tcsh bash gcc gcc-c++ perl python python-devel python-setuptools bash make gawk cmake3 python3

update-alternatives  --install /usr/bin/cmake cmake /usr/bin/cmake3 1
