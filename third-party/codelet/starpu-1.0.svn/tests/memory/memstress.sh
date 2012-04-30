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
filename=$TIMINGDIR/memstress.data

sizelist="4096 8192"
stresslist="0 50 100 150 200 250 300 350 400 450 500 550 600 650 655 660 665 670 675"
#stresslist="672"

trace_stress()
{
	memstress=$1

	export STARPU_NCPUS=0
	export STARPU_NCUDA=1
	export STRESS_MEM=$memstress

	line="$memstress"

	for size in $sizelist 
	do
		nblocks=$(($size / 1024))
		echo "Computing size $size with $memstress MB of memory LESS"
		
		echo "$ROOTDIR/examples/mult/dw_mult -x $size -y $size -z $size -nblocks $nblocks 2>/dev/null"
		timing=`$ROOTDIR/examples/mult/dw_mult -x $size -y $size -z $size -nblocks $nblocks 2>/dev/null`
	
		echo "size : $size memstress $memstress => $timing us"

		line="$line	$timing"

	done

	echo "$line" >> $filename
}

cd $ROOTDIR

make clean 1> /dev/null 2> /dev/null
make examples STARPU_ATLAS=1 CUDA=1 CPUS=3 1> /dev/null #2> /dev/null

cd $DIR

echo "#memstress $sizelist " > $filename

for memstress in $stresslist
do
	trace_stress $memstress;
done
