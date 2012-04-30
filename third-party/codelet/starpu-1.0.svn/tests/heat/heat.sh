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
make STARPU_ATLAS=1 CUDA=1 CPUS=4 1> /dev/null 2> /dev/null

BLOCKS=8

THETALIST="32 64 128 192 256 384"

echo "absolute wall time ..."
# perform all measurments first 
for theta in $THETALIST
do
	size=$(($theta*32))

	for cpus in 0
	do
		for cublas in 1
		do
			blocks=$BLOCKS
			filename=$TIMINGDIR/timing.$cpus.$cublas.$size.$blocks

			export STARPU_NCPUS=$cpus
			export STARPU_NCUDA=$cublas

			echo "size $size cpus $cpus cublas $cublas blocks $blocks" 
			$ROOTDIR/examples/heat -nthick 34 -ntheta $(($theta+2)) -nblocks $BLOCKS 2>/dev/null| tee $filename
		done
	done


	for cpus in 1 2 3 4
	do
		for cublas in 0 1
		do
			blocks=$BLOCKS
			filename=$TIMINGDIR/timing.$cpus.$cublas.$size.$blocks

			export STARPU_NCPUS=$cpus
			export STARPU_NCUDA=$cublas

			echo "size $size cpus $cpus cublas $cublas blocks $blocks" 
			$ROOTDIR/examples/heat -nthick 34 -ntheta $(($theta+2)) -nblocks $BLOCKS 2>/dev/null| tee $filename
		done
	done
done

# time
rm -f $DIR/time
for theta in $THETALIST
do
	size=$(($theta*32))
	line=`cat  $TIMINGDIR/timing.0.1.$size.$BLOCKS $TIMINGDIR/timing.4.0.$size.$BLOCKS $TIMINGDIR/timing.1.1.$size.$BLOCKS $TIMINGDIR/timing.3.1.$size.$BLOCKS $TIMINGDIR/timing.4.1.$size.$BLOCKS | tr '\n' '\t'`
	echo "$size	$line" >> $DIR/time
done

echo "speedup ..."

for blocks in 2 4 8 16 32
do	
	for theta in $THETALIST
	do
		size=$(($theta*32))

		export STARPU_NCPUS=4
		export STARPU_NCUDA=0

		echo "size $size cpus 4 cublas 0 blocks $blocks"
		filename=$TIMINGDIR/timing.4.0.$size.$blocks
		$ROOTDIR/examples/heat -nthick 34 -ntheta $(($theta+2)) -nblocks $blocks 2>/dev/null| tee $filename

		export STARPU_NCPUS=3
		export STARPU_NCUDA=1

		echo "size $size cpus 3 cublas 1 blocks $blocks"
		filename=$TIMINGDIR/timing.3.1.$size.$blocks
		$ROOTDIR/examples/heat -nthick 34 -ntheta $(($theta+2)) -nblocks $blocks 2>/dev/null| tee $filename
	done
done

# speedups 
for blocks in 2 4 8 16 32
do
	rm -f $DIR/speedup.$blocks
	for theta in $THETALIST
	do
		size=$(($theta*32))

		echo "$size	`cat $TIMINGDIR/timing.3.1.$size.$blocks`	`cat  $TIMINGDIR/timing.4.0.$size.$blocks`" >> $DIR/speedup.$blocks 
	done
done
