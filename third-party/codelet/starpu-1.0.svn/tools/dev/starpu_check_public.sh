#!/bin/bash
# Note: expects Coccinelle's spatch command n the PATH
# See: http://coccinelle.lip6.fr/

# StarPU --- Runtime system for heterogeneous multicore architectures.
#
# Copyright (C) 2011, 2012 Centre National de la Recherche Scientifique
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
greencolor=$(tput setaf 2)

functions=$(spatch -very_quiet -sp_file tools/dev/starpu_funcs.cocci $(find include -name '*.h'))
for func in $functions ; do
	fname=$(echo $func|awk -F ',' '{print $1}')
	location=$(echo $func|awk -F ',' '{print $2}')
	x=$(grep -rs "$fname" examples tests mpi starpufft gcc-plugin tools src/sched_policies)
	if test "$x" == "" ; then
	    echo "function ${redcolor}${fname}${stcolor} at location ${redcolor}$location${stcolor} is not used in any examples or tests"
	fi
done

echo

structs=$(grep "struct starpu" $(find include -name '*.h') | grep -v "[;|,|(|)]" | awk '{print $2}')
for struct in $structs ; do
    x=$(grep -rs "struct $struct" examples tests mpi starpufft gcc-plugin tools src/sched_policies)
    if test "$x" == "" ; then
	echo "struct ${redcolor}${struct}${stcolor} is not used in any examples or tests"
    fi
done

echo

enums=$(grep "enum starpu" $(find include -name '*.h') | grep -v "[;|,|(|)]" | awk '{print $2}')
for enum in $enums ; do
    x=$(grep -rs "enum $enum" examples tests mpi starpufft gcc-plugin tools src/sched_policies)
    if test "$x" == "" ; then
	echo "enum ${redcolor}${enum}${stcolor} is not used in any examples or tests"
    fi
done

echo

macros=$(grep "define\b" include/*|grep -v deprecated|grep "#" | grep -v "__" | sed 's/#[ ]*/#/g' | awk '{print $2}' | awk -F'(' '{print $1}' | sort|uniq)
for macro in $macros ; do
    x=$(grep -rs "$macro" examples tests mpi starpufft gcc-plugin tools src/sched_policies)
    if test "$x" == "" ; then
	echo "macro ${redcolor}${macro}${stcolor} is not used in any examples or tests"
    fi
done

