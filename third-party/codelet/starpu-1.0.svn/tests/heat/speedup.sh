#/bin/bash

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
cd $ROOTDIR

make clean 1> /dev/null 2> /dev/null
make STARPU_ATLAS=1 CPUS=16 1> /dev/null 2> /dev/null

echo "speedup ..."

for theta in 32 64 128 256 512 1024
do	
	size=$(($theta * 32))

	echo "# ncpus	time	reftime" >  $TIMINGDIR/speedup.$size

	for cpus in 1 2 4 6 8 10 12 14 16
	do
		export STARPU_NCPUS=$cpus

		echo "ncpus $cpus size $size"

		filename=$TIMINGDIR/timing.$cpus.$size
		$ROOTDIR/examples/heat -v2 -pin -nthick 34 -ntheta $(($theta+2)) -nblocks 16 2>/dev/null| tee $filename

		echo "$cpus	`cat $TIMINGDIR/timing.$cpus.$size`	`cat  $TIMINGDIR/timing.1.$size`" >> $TIMINGDIR/speedup.$size
	done
done
