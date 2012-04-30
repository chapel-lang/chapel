#!/bin/bash

# StarPU --- Runtime system for heterogeneous multicore architectures.
# 
# Copyright (C) 2009, 2010  Université de Bordeaux 1
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

maxiter=2

calibrate_point()
{
	strat=$1
	nblocks=$2
	prefetch=$3

	size=$(($nblocks * 1024))

	echo "STARPU_CALIBRATE size : $size / blocks : $nblocks strat -> $strat prefetch -> $prefetch"

	rm -f $SAMPLINGDIR/*
	
	for iter in `seq 1 $maxiter`
	do
		echo "$iter / $maxiter"
		export STARPU_SCHED=$strat
		export STARPU_CALIBRATE=1
		export STARPU_PREFETCH=$prefetch
		val=`$ROOTDIR/examples/heat/heat -pin -nblocks $nblocks -size $size -v3 2> /dev/null`
		echo "$val"
	done

}


DIR=$PWD
ROOTDIR=$DIR/../..
TIMINGDIR=$DIR/timings_dmda/
SAMPLINGDIR=$DIR/sampling/
export STARPU_PERF_MODEL_DIR=$SAMPLINGDIR

mkdir -p $TIMINGDIR
mkdir -p $SAMPLINGDIR

#schedlist="ws no-prio greedy prio dm random"
#schedlist="random random random random"

export STARPU_NCUDA=3
export STARPU_NCPUS=8 

#sizelist="2048 4096 6144 8192 10240 12288 14336 16384 24576 28672 30720"
sizelist=`seq 2048 2048 30720`

outputfile=dmda.data

rm -f $outputfile

for size in $sizelist
do
	nblocks=$(($size / 1024))

	calibrate_point "dm" $nblocks 1

	export STARPU_SCHED="dm"
	export STARPU_CALIBRATE=1
	export STARPU_PREFETCH=1
	valdm=$($ROOTDIR/examples/heat/heat -pin -size $size -nblocks $nblocks -v3 2> logdm)

	calibrate_point "dmda" $nblocks 1

	export STARPU_SCHED="dmda"
	export STARPU_CALIBRATE=1
	export STARPU_PREFETCH=1
	valdmda=$($ROOTDIR/examples/heat/heat -pin -size $size -nblocks $nblocks -v3 2> logdmda)
	
	dmmiss=`grep "TOTAL MSI" logdm|sed -e "s/.*miss.*[1-9]* (\(.*\) %)/\1/"`
	dmtotal=`grep "TOTAL transfers" logdm|sed -e "s/TOTAL transfers \(.*\) MB/\1/"`
	dmdamiss=`grep "TOTAL MSI" logdmda|sed -e "s/.*miss.*[1-9]* (\(.*\) %)/\1/"`
	dmdatotal=`grep "TOTAL transfers" logdmda|sed -e "s/TOTAL transfers \(.*\) MB/\1/"`

	echo "$size	$dmmiss	$dmdamiss	$dmtotal	$dmdatotal	$valdm	$valdmda" >> $outputfile
	echo "$size	$dmmiss	$dmdamiss	$dmtotal	$dmdatotal	$valdm	$valdmda" 
done

