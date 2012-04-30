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


set term postscript landscape color 22 
set output "memstress.ps"
set xlabel "Memory Pressure (MB)"
set ylabel "execution time degradation (%)"
set grid y
set key left top box
set datafile missing 'x'
plot "timings/memstress.data" usi 1:(( 100*(($2 / 2130) - 1))) with linespoint title "matrix size : 4096"  ,\
     "timings/memstress.data" usi 1:(( 100*(($3 / 16420) - 1) )) with linespoint title "8192"
