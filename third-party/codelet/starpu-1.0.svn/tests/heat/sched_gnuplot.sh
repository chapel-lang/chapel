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

prefix=sched
outputfile=$prefix.data

find timings_sched/* -name "$prefix.*" > filelist

grainlist=`sed -e "s/.*$prefix\.\(.*\)\.\(.*\)$/\1/" filelist |sort -n|uniq|xargs`
sizelist=`sed -e "s/.*$prefix\.\(.*\)\.\(.*\)$/\2/" filelist |sort -n|uniq`

schedlist="0.greedy 0.dm 1.dm 1.dmda"

# Make some header 
line="#SIZE	"
for sched in $schedlist
do
	line="$line	$sched"
done
echo "$line" > $outputfile

for size in $sizelist
do
	line="$size	"

	for sched in $schedlist
	do
		# Compute Average value ...

		if test -f timings_sched/$prefix.$sched.$size; then
			# file does exists 
			filename=timings_sched/$prefix.$sched.$size

			# how many samples do we have ?
			nsample=`cat $filename | wc -w`
			if test $nsample -ge 1; then
				sum=0
				for i in `cat $filename | xargs`
				do
					sum=$(echo "$sum + $i"|bc -l)
				done
				
				# average execution time is ...
				mean=$(echo "$sum / $nsample"|bc -l)

				# in Flop/s this is 2*size^3/3
				gflops=$(echo "2.0 * $size * $size * $size / (3000000 * $mean)"|bc -l)

				# just make this a bit prettier ..
				gflops=`echo $gflops | sed -e "s/\(.*\.[0-9][0-9]\).*$/\1/"` 

				line="$line     $gflops"
			else
				# we have no valid sample even if the file exists
				line="$line     x"
			fi 
		else
			# file does not exist
			line="$line     x"
		fi
		
		line="$line	"
	done

	echo "$line" >> $outputfile
done

gnuplotline="plot "
cnt=2
echo "grainlist ... $grainlist"
for sched in $schedlist
do
	if test $cnt -ne 2; then
		# i hate gnuplot :)
		gnuplotline="$gnuplotline , "
	fi
	gnuplotline="$gnuplotline \"$outputfile\" usi 1:$cnt with linespoint title \"\($grain x $grain\)\" lt rgb \"black\" "
	cnt=$(($cnt+1))
done

gnuplot > /dev/null << EOF

set term postscript eps enhanced color
set output "$prefix.eps"

set datafile missing 'x'
set pointsize 0.75
#set title "Impact of granularity"
set grid y
set grid x
set xrange [0:49152]
#set logscale x
set xtics 8192,8192,65536
set key invert box right bottom title "Scheduling policy"
set size 0.65

set xlabel "Matrix size"
set ylabel "GFlop/s"

plot "$outputfile" usi 1:2 with linespoint title "greedy" lt rgb "black" lw 2,\
	"$outputfile" usi 1:3 with linespoint title "heft-tm" lt rgb "black" lw 2,\
	"$outputfile" usi 1:4 with linespoint title "heft-tm-pr" lt rgb "black" lw 2,\
	"$outputfile" usi 1:5 with linespoint title "heft-tmdp-pr" lt rgb "black" lw 2

EOF

