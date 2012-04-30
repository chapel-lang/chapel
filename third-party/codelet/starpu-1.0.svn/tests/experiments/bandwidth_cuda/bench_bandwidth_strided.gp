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
set output "bench_bandwidth_strided.eps"
set title "CUDA Bandwidth"
set logscale x
set xlabel "Size (Bytes/4)"
set ylabel "Bandwidth (MB/s)"

# plot ".results/htod-pin.data" with linespoint	title "Host to Device (pinned)" ,\
#      ".results/htod-pin.32.data"  with linespoint   title "stride 32" ,\
#      ".results/htod-pin.128.data"  with linespoint   title "stride 128" ,\
#      ".results/htod-pin.512.data"  with linespoint   title "stride 512" ,\
#      ".results/htod-pin.1024.data"  with linespoint   title "stride 1024" ,\
#      ".results/htod-pin.2048.data"  with linespoint   title "stride 2048" ,\
#      ".results/htod-pin.4096.data"  with linespoint   title "stride 4096" ,\
#      ".results/htod-pin.8192.data"  with linespoint   title "stride 8192" 
# 


plot ".results/htod-pin.data" with linespoint	title "Host to Device (pinned)" ,\
     ".results/htod-pin.2.data"  with linespoint   title "stride 2" ,\
     ".results/htod-pin.4.data"  with linespoint   title "stride 4",\
     ".results/htod-pin.8.data"  with linespoint   title "stride 8" ,\
     ".results/htod-pin.16.data"  with linespoint   title "stride 16" ,\
     ".results/htod-pin.32.data"  with linespoint   title "stride 32" ,\
     ".results/htod-pin.64.data"  with linespoint   title "stride 64" ,\
     ".results/htod-pin.128.data"  with linespoint   title "stride 128" ,\
     ".results/htod-pin.256.data"  with linespoint   title "stride 256" ,\
     ".results/htod-pin.512.data"  with linespoint   title "stride 512" ,\
     ".results/htod-pin.1024.data"  with linespoint   title "stride 1024" ,\
     ".results/htod-pin.2048.data"  with linespoint   title "stride 2048" ,\
     ".results/htod-pin.4096.data"  with linespoint   title "stride 4096" ,\
     ".results/htod-pin.8192.data"  with linespoint   title "stride 8192" 


