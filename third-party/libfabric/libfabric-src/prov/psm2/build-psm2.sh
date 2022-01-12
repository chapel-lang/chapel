#!/bin/sh
########################################################################
# This is a sample script for building the psm2 provider with three
# different options:
#
# lib:  Link against external libpsm2.so;
# src:  With PSM2 library source compiled in;
# lto:  With PSM2 library source compiled in and with link-time
#       optimization enabled.
#
# Please run the script from the top level directory of the source repo.
#
# The "psm", "usnic", and "verbs" providers are disabled to reduce the
# building time. They can be enabled as needed.
#
# Please check that the following variables are either set to appropriate
# values or can use the default values.
#
# PREFIX     Destination for installation
# STAGE	     Name for sub-directory used for building
# PSM2_HOME  Location for PSM2 header and libraries (for "lib" build)
# PSM2_SRC   Location for PSM2 source code (for "src" and "lto" builds)
########################################################################

if [ $# -eq 0 ]; then
	echo Usage: $0 '[lib|src|lto]'
	exit 1
fi

case $1 in
  lib)	options=""
	cflags=""
	ldflags=""
	;;
  src)	options="--with-psm2-src=${PSM2_SRC:-$HOME/scm/opa-psm2}"
	cflags='CFLAGS="-O3"'
	ldflags=""
	;;
  lto)	options="--with-psm2-src=${PSM2_SRC:-$HOME/scm/opa-psm2}"
	cflags='CFLAGS="-O3 -flto -ffat-lto-objects -msse4.2"'
	ldflags='LDFLAGS="-fuse-linker-plugin -flto-partition=none"'
	;;
*)	echo Invalid option: "$1"
	exit 1
	;;
esac

if [ ! -f ./autogen.sh ]; then
	echo Please run the script from the top level directory.
	exit 1
fi

./autogen.sh && \
cd ${STAGE:-stage} && \
eval ../configure \
	$cflags $ldflags $options \
	--prefix=${PREFIX:-$HOME/install/ofi} \
	--enable-psm2=${PSM2_HOME:-yes} \
	--disable-psm \
	--disable-usnic \
	--disable-verbs && \
make && make install

