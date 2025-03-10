#!/bin/sh

apt-get update
DEBIAN_FRONTEND=noninteractive apt-get install -y gcc g++ m4 perl python3 python3-dev bash make mawk git pkg-config cmake
