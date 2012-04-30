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

#suffix="-5800"
suffix=""

TIMINGDIR=$PWD/timing$suffix/

sizelist=""
schedlist=""
outputfiles=""

for file in `find $TIMINGDIR -type f`
do
	name=`basename $file`
	size=`echo $name|sed -e "s/.*\.\(.*\)\..*/\1/"`
	sched=`echo $name|sed -e "s/\(.*\)\..*\(\..*\)/\1/"`
	sizelist="$sizelist $size"
	schedlist="$schedlist $sched"
done

sizelist=`echo $sizelist|tr " " "\n" |sort -n|uniq`
schedlist=`echo $schedlist|tr " " "\n" |sort|uniq`

for prio in `seq 0 1`
do
for sched in $schedlist
do
	outputfile=output$suffix.$sched.$prio
	outputfiles="$outputfiles $outputfile"

	rm -f $outputfile

	for size in $sizelist
	do
		filename=$TIMINGDIR/$sched.$size.$prio
		if test -f $filename; then
			# file exists
			sum=0
			nsample=0
			
			for val in `cat $filename`
			do
				nsample=$(($nsample + 1))
				sum=$(echo "$sum + $val"|bc -l)
			done

			avg=$(echo "$sum / $nsample"|bc -l)
			gflops=$(echo "$size * $size * $size / ( $avg * 3000000)"|bc -l)
			echo "$size	$gflops" >> $outputfile

		else
			# file does not exist
			echo "$size	x" >> $outputfile 
		fi
	done
done
done

gnuplotline=""

for outputfile in $outputfiles
do
	line=" \"$outputfile\" with linespoint"
	gnuplotline="$gnuplotline $line @"
done

gnuplotarg=`echo $gnuplotline|tr '@' ','|sed -e "s/\(.*\),/\1/"`

prefix=cholesky

gnuplot > /dev/null << EOF
set term postscript eps enhanced color
set output "$prefix$suffix.eps"

set datafile missing 'x'

set pointsize 0.75
#set title "Impact of granularity"
set grid y
set grid x
set xrange [0:49152]
#set logscale x
#set xtics 8192,8192,65536
#set key invert box right bottom title "Scheduling policy"
#set size 0.65

set xlabel "Matrix size"
set ylabel "GFlop/s"

plot $gnuplotarg

EOF
