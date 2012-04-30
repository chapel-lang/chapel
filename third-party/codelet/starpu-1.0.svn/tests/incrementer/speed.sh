#!/bin/bash

# StarPU --- Runtime system for heterogeneous multicore architectures.
# 
# Copyright (C) 2010  Université de Bordeaux 1
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

# We compute the duration of the experiment for multiple powers of 2

i=1

examplebindir=../../examples/incrementer/

rm -f .perftable

max=20

for logi in `seq 0 $max`
do
	$examplebindir/incrementer $i 2> .tmpperf

	grep "ms" .tmpperf
	grep "ms" .tmpperf | sed -e "s/^\(.*\) elems took \(.*\) ms$/\1	\2/" >> .perftable 

	i=$((2 * $i))
done

gnuplot > /dev/null << EOF
set term postscript eps enhanced color
set output "incrementer.eps"

set xlabel "Number of iterations"
set ylabel "Execution time (ms)"

set logscale x
set logscale y

plot ".perftable" using 1:2 with linespoint title "Time"
EOF
