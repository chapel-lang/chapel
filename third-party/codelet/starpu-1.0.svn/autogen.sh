#!/bin/sh

# StarPU --- Runtime system for heterogeneous multicore architectures.
# 
# Copyright (C) 2010  Université de Bordeaux 1
# Copyright (C) 2010  Centre National de la Recherche Scientifique
# 
# StarPU is free software; you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation; either version 2.1 of the License, or (at
# your option) any later version.
# 
# StarPU is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# 
# See the GNU Lesser General Public License in COPYING.LGPL for more details.

if ! libtool --version > /dev/null
then
	# Perhaps we are on a Mac
	if ! glibtool --version > /dev/null
	then
	echo "GNU Libtool is missing, please install it."
	exit 1
	fi
fi
autoreconf -ivf -I m4

