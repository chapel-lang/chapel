#!/usr/bin/gnuplot -persist

# StarPU --- Runtime system for heterogeneous multicore architectures.
# 
# Copyright (C) 2008, 2009  Université de Bordeaux 1
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


set term postscript eps enhanced color
set output "sched.eps"
set title "Matrix multiplication : impact of the scheduling policy"
set xlabel "matrices size"
set ylabel "GFlops"
set logscale x
set key right bottom
set datafile missing 'x'
plot "timings/gflops.data.greedy" usi 1:(2*$1*$1*$1 / ($2* 1000000)) with lines title "greedy"  ,\
     "timings/gflops.data.dm" usi 1:(2*$1*$1*$1 / ($2* 1000000)) with lines title "dm"
