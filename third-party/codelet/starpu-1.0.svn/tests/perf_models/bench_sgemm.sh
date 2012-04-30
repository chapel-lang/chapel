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

DIR=$PWD
ROOTDIR=$DIR/../..
BUILDDIR=$PWD/build/
INSTALLDIR=$PWD/local/
PERFDIR=$DIR/sampling/

make -C ../../ distclean

mkdir -p $PERFDIR
mkdir -p $BUILDDIR
cd $BUILDDIR
$DIR/../../configure -C --prefix=$INSTALLDIR --with-goto-dir=/home/gonnet/These/Libs/GotoBLAS/GotoBLAS/ --enable-verbose

make -j 10
make install

sizelist="16 32 48 64 96 128 192 256 384 512 1024 2048 4096"

cpu_output=$DIR/output.cpu
gpu_output=$DIR/output.gpu

rm -f $cpu_output
rm -f $gpu_output

export STARPU_WORKERS_CPUID="2"
export STARPU_CALIBRATE=1
export STARPU_SCHED="dm"

# benchmark GotoBLAS
for size in $sizelist;
do
	niter=1000
	if test $size -ge 512; then
		niter=20
	fi
	if test $size -ge 2048; then
		niter=5
	fi

	echo "GotoBLAS -> size $size niter $niter"
	timing=`STARPU_NCPUS=1 STARPU_NCUDA=0 $INSTALLDIR/lib/starpu/examples/dw_mult_no_filters -x $size -y $size -z $size -nblocks 1 -iter $niter 2> /dev/null`
	echo "$size	$timing	$niter" >> $cpu_output
done

# benchmark CUBLAS
for size in $sizelist;
do
	niter=2500
	if test $size -ge 512; then
		niter=250
	fi

	if test $size -ge 2048; then
		niter=25
	fi

	echo "CUBLAS -> size $size niter $niter"
	timing=`STARPU_NCPUS=0 STARPU_NCUDA=1 $INSTALLDIR/lib/starpu/examples/dw_mult_no_filters -x $size -y $size -z $size -nblocks 1 -iter $niter 2 -pin 2> /dev/null`
	echo "$size	$timing	$niter" >> $gpu_output
done

gnuplot > /dev/null << EOF

set term postscript eps enhanced color
set output "bench_sgemm.eps"

set logscale x
set logscale y

plot "$cpu_output" usi 1:(\$2/\$3) with linespoint, \
	"$gpu_output" usi 1:(\$2/\$3) with linespoint

EOF

