#!/bin/bash
# Note: expects Coccinelle's spatch command n the PATH
# See: http://coccinelle.lip6.fr/

# StarPU --- Runtime system for heterogeneous multicore architectures.
#
# Copyright (C) 2011 Centre National de la Recherche Scientifique
# Copyright (C) 2011 Institut National de Recherche en Informatique et Automatique
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

stcolor=$(tput sgr0)
redcolor=$(tput setaf 1)

filese=$(find examples -type f -not -path "*svn*" -name '*.c')
filest=$(find tests -type f -not -path "*svn*" -name '*.c')

for file in $filest $filese ; do
    handles=$(spatch -very_quiet -sp_file tools/dev/starpu_check_register.cocci $file)
    if test "x$handles" != "x" ; then
	for handle in $handles; do
	    echo "$handle"
	    register=$(echo $handle|awk -F ',' '{print $1}')
	    location=$(echo $handle|awk -F ',' '{print $2}')
	    echo "data handle ${redcolor}${register}${stcolor} registered at location $location does not seem to be properly unregistered"
	done
    fi
done
