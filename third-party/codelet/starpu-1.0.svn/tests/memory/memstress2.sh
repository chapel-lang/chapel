#!/bin/bash


# StarPU --- Runtime system for heterogeneous multicore architectures.
# 
# Copyright (C) 2008, 2009, 2010  Université de Bordeaux 1
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

DIR=$PWD
ROOTDIR=$DIR/../..
TIMINGDIR=$DIR/timings/
mkdir -p $TIMINGDIR
filename=$TIMINGDIR/memstress2.data

sizelist="512 1024 2048 4096 8192 16384"
stresslist="0 350"
#stresslist="672"

trace_stress()
{
	size=$1

	line="$size"

	for stress in $stresslist
	do
		export STRESS_MEM=$stress

		nblocks=$(($size / 1024))
		echo "Computing size $size with $stress MB of memory LESS"

		
		echo "$ROOTDIR/examples/mult/dw_mult -x $size -y $size -z $size -nblocks $nblocks 2>/dev/null"
		timing=`$ROOTDIR/examples/mult/dw_mult -x $size -y $size -z $size -nblocks $nblocks 2>/dev/null`
	
		echo "size : $size memstress $stress => $timing us"

		line="$line	$timing"

	done

	echo "$line" >> $filename
}

cd $ROOTDIR

make clean 1> /dev/null 2> /dev/null
make examples STARPU_ATLAS=1 CUDA=1 CPUS=0 1> /dev/null 2> /dev/null

cd $DIR

echo "#size $stresslist " > $filename

for size in $sizelist
do
	trace_stress $size;
done
