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

maxiter=5
ROOTDIR=../../
TIMINGDIR=$PWD/timing/

export STARPU_WORKERS_CUDAID="1"

trace_sched()
{
	sched=$1
	use_prio=$2

	export STARPU_SCHED=$sched

	for blocks in `seq 24 2 24`
	do
		size=$(($blocks*1024))
	
		echo "size : $size"
	
		OPTIONS="-pin -nblocks $blocks -size $size"

		if [ $use_prio -eq 0 ]
		then
			OPTIONS="$OPTIONS -no-prio"
		fi
		
		filename=$TIMINGDIR/sched.$STARPU_SCHED.$size.$use_prio

		for iter in `seq 1 $maxiter`
		do
			echo "$iter / $maxiter"
			echo "$ROOTDIR/examples/cholesky/dw_cholesky $OPTIONS 2> /dev/null"
			val=`$ROOTDIR/examples/cholesky/dw_cholesky $OPTIONS 2> /dev/null`
			echo "$val" >> $filename
			echo "$val"
		done
	done
}

schedlist='dm dm dm dm greedy dm'

export STARPU_NCUDA=1
export STARPU_CALIBRATE=1

mkdir -p $TIMINGDIR

# calibrate
for i in `seq 1 5` 
do
STARPU_SCHED="dm" $ROOTDIR/examples/cholesky/dw_cholesky -nblocks 16 -size 16384 2> /dev/null
done

for sched in $schedlist
do
	echo "sched : $sched"

	trace_sched $sched 0;
	trace_sched $sched 1;
done
