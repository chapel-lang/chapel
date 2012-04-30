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
set output "gflops_sched.eps"
set title "LU Decomposition : scheduling strategies"
set grid y
set key box
set xlabel "problem size (unknowns)"
set ylabel "GFlop/s"
set logscale x
#set pointsize	2
set key right bottom
set datafile missing 'x'
plot "timings/gflops.merged.data" usi 1:(2*$1*$1*$1 / (3*$2* 1000000)) with linespoint title "greedy"  ,\
     "timings/gflops.merged.data" usi 1:(2*$1*$1*$1 / (3*$4* 1000000)) with linespoint title "prio" 	    ,\
     "timings/gflops.merged.data" usi 1:(2*$1*$1*$1 / (3*$6* 1000000)) with linespoint title "ws" 

set output "gflops_sched_gain.eps"
set title "LU Decomposition : scheduling strategies : gain"
set grid y
set key box
set xlabel "problem size (unknowns)"
set ylabel "Gain"
set logscale x
#set pointsize	2
set key right bottom
set datafile missing 'x'
plot "timings/gflops.merged.data" usi 1:(100*(($2 / $4)-1)) with linespoint title "gain prio"	,\
	"timings/gflops.merged.data" usi 1:(100*(($2 / $6)-1)) with linespoint title "gain ws"    
