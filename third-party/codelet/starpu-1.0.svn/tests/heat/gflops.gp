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
set output "gflops.eps"
#set title "Facto LU : impact of granularity"
set grid y
set key box
set xlabel "problem size (unknowns)"
set ylabel "GFlop/s"
set logscale x
#set pointsize	2
set key right bottom
set datafile missing 'x'
plot "timings/gflops.data" usi 1:(2*$1*$1*$1 / (3*$2* 1000000)) with linespoint lt 3 title "block size : 128"  ,\
     "timings/gflops.data" usi 1:(2*$1*$1*$1 / (3*$3* 1000000)) with linespoint title "block size : 256"  ,\
     "timings/gflops.data" usi 1:(2*$1*$1*$1 / (3*$4* 1000000)) with linespoint title "block size : 512"  ,\
     "timings/gflops.data" usi 1:(2*$1*$1*$1 / (3*$5* 1000000)) with linespoint title "block size : 1024"
