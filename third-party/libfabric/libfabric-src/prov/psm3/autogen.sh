#! /bin/sh

if test ! -f src/psmx3.h; then
	echo You really need to run this script in the prov psm3 directory in git
	exit 1
fi

set -x
autoreconf -ivf

