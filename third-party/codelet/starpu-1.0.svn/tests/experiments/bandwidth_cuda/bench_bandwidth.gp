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
set output "bench_bandwidth.eps"
set title "CUDA Bandwidth"
set logscale x
set xlabel "Size (Bytes)"
set ylabel "Bandwidth (MB/s)"

plot ".results/htod-pin.data" with linespoint	title "Host to Device (pinned)",\
     ".results/dtoh-pin.data" with linespoint   title "Device to Host (pinned)"

