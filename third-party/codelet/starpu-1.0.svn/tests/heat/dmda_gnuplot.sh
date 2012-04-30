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

outputfile=dmda.data

gnuplot > /dev/null << EOF

set term postscript eps enhanced color
set output "dmda.eps"

#set pointsize 0.75
set grid y
set grid x
#set logscale x
set size 0.65

set xlabel "Matrix size"

set xtics 0,8192,49152

set size 0.7,0.8

set multiplot

set size 0.7,0.4
set origin 0.0,0.0
set ylabel "Data accesses requiring\n a memory transfer"
set format y "%.0f %%"
set key box right top title "Policy"
plot "$outputfile" usi 1:2 with linespoint lt rgb "black"  lw 4 title "heft-tm-pr" ,\
     "$outputfile" usi 1:3 with linespoint lt rgb "black"  lw 4 title "heft-tmdp-pr"

set size 0.7,0.4
set origin 0.0,0.4
set ylabel "Avg. activity on bus (GB/s)\n"
set format y "%.1f"
set key  box right bottom title "Policy"
plot "$outputfile" usi 1:((\$4/\$6)) with linespoint lt rgb "black" lw 4 title "heft-tm-pr",\
     "$outputfile" usi 1:((\$5/\$7)) with linespoint lt rgb "black"  lw 4 title "heft-tmdp-pr"

unset multiplot

EOF

