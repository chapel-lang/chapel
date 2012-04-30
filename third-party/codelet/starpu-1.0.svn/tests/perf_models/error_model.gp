#!/usr/bin/gnuplot -persist

# StarPU --- Runtime system for heterogeneous multicore architectures.
# 
# Copyright (C) 2009  Université de Bordeaux 1
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
set output "model_error.eps"

set yrange [0.05:100]
set xrange [4:10000]

set grid y
set grid x

set logscale y
set logscale x

#set title "Cholesky on Cell (PS3)"
set xlabel "Number of samples"
set ylabel "Prediction error"
set grid


set ytics (0.01, 0.1, 1, 5,10,25,50,100)
set xtics (10, 100, 1000, 10000)

set format y "%.1f %%"
set format x "10^{%L}"

set key title "Execution time Prediction Error (%)" 

set size 0.75

plot "gnuplot.data" usi 1:($2*100) with linespoint pt -1 lt 1 lw 3 title "CPUs"	,\
     "gnuplot.data" usi 3:($4*100) with linespoint pt -1 lt 2 lw 3 title "GPU"
