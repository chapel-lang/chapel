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
set output "time.eps"
set title "Facto LU"
set logscale y
set xlabel "problem size (unknowns)"
set ylabel "execution time (ms)"
plot "time" usi 1:2 with lines title "0 cpu + 1 gpu",\
     "time" usi 1:3 with lines title "4 cpus",\
     "time" usi 1:4 with lines title "1 cpu + 1 gpu",\
     "time" usi 1:5 with lines title "3 cpus + 1 gpu",\
     "time" usi 1:6 with lines title "4 cpus + 1 gpu"
