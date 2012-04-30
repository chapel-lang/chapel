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
SAMPLINGDIR=$DIR/sampling/
TIMINGDIR=$DIR/timing/

maxiter=5
MINSIZE=$((30*1024))
MAXSIZE=$((31*1024))

trace_granularity()
{
	grain=$1

	echo "GRAIN $grain"

	#minblocks=1
	minblocks=$(($MINSIZE/$grain))
	#maxblocks=2
	maxblocks=$(($MAXSIZE/$grain))
	
	if test $maxblocks -ge 128; then
		maxblocks=128
	fi

	#step=2
#	step=$((2048/$grain))
	step=$((4096/$grain))

	for blocks in `seq $minblocks $step $maxblocks`
	do
		size=$(($blocks*$grain))
		
		echo "size : $size (grain $grain nblocks $blocks)"
	
		calibrate_grain $grain $size
	
		OPTIONS="-pin -nblocks $blocks -size $size -v3"
		
		filename=$TIMINGDIR/granularity.$grain.$size
		#rm -f $filename
		
		for iter in `seq 1 $maxiter`
		do
			echo "$iter / $maxiter"
			 val=`STARPU_NCPUS=8 STARPU_NCUDA=3 STARPU_SCHED="dmda" STARPU_PREFETCH=1 STARPU_CALIBRATE=1 $ROOTDIR/examples/heat/heat $OPTIONS 2> /dev/null`
			 echo "$val"
			 echo "$val" >> $filename
		done
	done
}

trace_granularity_hybrid()
{
	grain=$1

	echo "GRAIN $grain"

	#minblocks=1
	minblocks=$(($MINSIZE/$grain))
	#maxblocks=2
	maxblocks=$(($MAXSIZE/$grain))
	
	if test $maxblocks -ge 64; then
		maxblocks=64
	fi

	#step=2
	step=$((2048/$grain))

	for blocks in `seq $minblocks $step $maxblocks`
	do
		size=$(($blocks*$grain))
		
		ntheta=$(( $(($size/32)) + 2))
	
		echo "size : $size (grain $grain nblocks $blocks)"
	
		OPTIONS="-pin -nblocks $blocks -ntheta $ntheta -nthick 34 -v4"
		
		filename=$TIMINGDIR/hybrid.$grain.$size
		#rm -f $filename
		
		for iter in `seq 1 $maxiter`
		do
			echo "$iter / $maxiter"
			 val=`STARPU_SCHED="dmda" STARPU_PREFETCH=1 STARPU_CALIBRATE=1 $ROOTDIR/examples/heat/heat $OPTIONS 2> /dev/null`
			 echo "$val"
			 echo "$val" >> $filename
		done
	done
}



calibrate_grain()
{
	grain=$1
	size=$2

	echo "Calibrating grain $grain size $size ($blocks blocks)"

	rm -f $SAMPLINGDIR/*

	OPTIONS="-pin -nblocks $blocks -size $size -v3"

	STARPU_NCUDA=3 STARPU_NCPUS=8 STARPU_CALIBRATE=1 STARPU_SCHED="dm" $ROOTDIR/examples/heat/heat $OPTIONS 2> /dev/null 
	STARPU_NCUDA=3 STARPU_NCPUS=8 STARPU_CALIBRATE=1 STARPU_PREFETCH=1 STARPU_SCHED="dmda" $ROOTDIR/examples/heat/heat $OPTIONS 2> /dev/null
	STARPU_NCUDA=3 STARPU_NCPUS=8 STARPU_CALIBRATE=1 STARPU_PREFETCH=1 STARPU_SCHED="dmda" $ROOTDIR/examples/heat/heat $OPTIONS 2> /dev/null
	STARPU_NCUDA=3 STARPU_NCPUS=8 STARPU_CALIBRATE=1 STARPU_PREFETCH=1 STARPU_SCHED="dmda" $ROOTDIR/examples/heat/heat $OPTIONS 2> /dev/null
	STARPU_NCUDA=3 STARPU_NCPUS=8 STARPU_CALIBRATE=1 STARPU_PREFETCH=1 STARPU_SCHED="dmda" $ROOTDIR/examples/heat/heat $OPTIONS 2> /dev/null
	STARPU_NCUDA=3 STARPU_NCPUS=8 STARPU_CALIBRATE=1 STARPU_PREFETCH=1 STARPU_SCHED="dmda" $ROOTDIR/examples/heat/heat $OPTIONS 2> /dev/null
}

mkdir -p $TIMINGDIR
mkdir -p $SAMPLINGDIR
#rm  -f $SAMPLINGDIR/*

#grainlist="64 128 256 512 768 1024 1536 2048"

#grainlist="1024 2048 1024 512 256"
grainlist="256 32 64 128 256 512 1024 2048 4096"

export STARPU_PERF_MODEL_DIR=$SAMPLINGDIR

cd $ROOTDIR

cd $DIR

# calibrate (sampling)
#	for grain in $grainlist
#	do
#	 	calibrate_grain $grain;
#	#  	calibrate_grain $(( $grain / 2));
#	done

# perform the actual benchmarking now
for grain in $grainlist
do
#	trace_granularity_hybrid $grain;
	trace_granularity $grain;	
#	trace_granularity_nomodel $grain;
done
