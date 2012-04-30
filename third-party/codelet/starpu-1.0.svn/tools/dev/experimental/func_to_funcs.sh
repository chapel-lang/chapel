#!/bin/sh

# StarPU --- Runtime system for heterogeneous multicore architectures.
#
# Copyright (C) 2011  Centre National de la Recherche Scientifique
# Copyright (C) 2011  Institut National de Recherche en Informatique et Automatique
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

path_to_cocci_scripts=tools/dev/experimental/
dir=${1:-.}

if [ ! -d $dir ]
then
	echo "${path_to_cocci_scripts} does not exist...";
	exit 1
fi

#cpu_func => cpu_funcs
spatch -sp_file ${path_to_cocci_scripts}/cpu_func_to_cpu_funcs.cocci -dir $dir -in_place -very_quiet

# cuda_func => cuda_funcs
sed 's/cpu_funcs/cuda_funcs/;
    s/cpu_func/cuda_func/;
    s/STARPU_MULTIPLE_CPU_IMPLEMENTATIONS/STARPU_MULTIPLE_CUDA_IMPLEMENTATIONS/' \
    ${path_to_cocci_scripts}/cpu_func_to_cpu_funcs.cocci > /tmp/cuda.cocci

spatch -sp_file /tmp/cuda.cocci -dir $dir -in_place -very_quiet
rm /tmp/cuda.cocci

# opencl_func => opencl_funcs
sed 's/cpu_funcs/opencl_funcs/;
    s/cpu_func/opencl_func/;
    s/STARPU_MULTIPLE_CPU_IMPLEMENTATIONS/STARPU_MULTIPLE_OPENCL_IMPLEMENTATIONS/'  \
    ${path_to_cocci_scripts}/cpu_func_to_cpu_funcs.cocci > /tmp/opencl.cocci

spatch -sp_file /tmp/opencl.cocci -dir $dir -in_place -very_quiet
rm /tmp/opencl.cocci

exit 0

