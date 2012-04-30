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
set output "memstress2.ps"
set xlabel "Problem size"
set ylabel "execution time"
set logscale x
set key left top
set datafile missing 'x'
plot "timings/memstress2.data" usi 1:2 with lines title "reference"  ,\
     "timings/memstress2.data" usi 1:3 with lines title "350 MB"
