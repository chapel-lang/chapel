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


#ampllist="0.0 0.1 0.2 0.25 0.3 0.4 0.50 0.6 0.7 0.75 0.8 0.9 0.95 1.0 "
#ampllist="1.0 0.50 0.25 0.1 0.0"
ampllist="1.0 0.5"

maxiter=1
MAXCPU=3

trace_perturbation()
{
	export STARPU_SCHED="dm"

	for blocks in `seq 12 2 24`
	do

		ntheta=$(( $(($blocks*32)) + 2))
		size=$(( $(($blocks*32)) * 32))
	
		echo "size : $size"
	
		OPTIONS="-pin -v2 -nblocks $blocks -ntheta $ntheta -nthick 34"
		
		cd $ROOTDIR
		filename=$TIMINGDIR/pertubate.$size.$AMPL
		#rm -f $filename
		make clean 1> /dev/null 2> /dev/null
		make examples -j STARPU_ATLAS=1 CPUS=$MAXCPU CUDA=1 PERTURB_AMPL=$AMPL 1> /dev/null 2> /dev/null
		cd $DIR

		if [ $size -le 16384 ]
		then
			nsamples=$maxiter
		else
			nsamples=1
		fi
		
		for iter in `seq 1 $nsamples`
		do
			echo "$iter / $nsamples"
			 val=`$ROOTDIR/examples/heat/heat $OPTIONS 2> /dev/null`
			 echo "$val" >> $filename
		done
	done
}

DIR=$PWD
ROOTDIR=$DIR/../..
TIMINGDIR=$DIR/timing_perturbate/
mkdir -p $TIMINGDIR

for ampl in $ampllist
do
	export AMPL=$ampl
	echo "ampl : $AMPL"

	trace_perturbation;
done
