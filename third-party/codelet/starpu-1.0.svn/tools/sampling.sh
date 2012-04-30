#!/bin/bash

# StarPU --- Runtime system for heterogeneous multicore architectures.
# 
# Copyright (C) 2008, 2009, 2010  Université de Bordeaux 1
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


inputfile=$1

archlist=`cut -f 1 $inputfile | sort | uniq | xargs` 
hashlist=`cut -f 2 $inputfile | sort | uniq | xargs` 

# extract subfiles from the history file
for arch in $archlist
do
	for h in $hashlist
	do
		echo "pouet $arch - $h "
		grep "^$arch	$h" $inputfile > $inputfile.$arch.$h
	done
done

# create the gnuplot file

gpfile=$inputfile.gp

echo "#!/usr/bin/gnuplot -persist" 		> $gpfile
echo "set term postscript eps enhanced color" 	>> $gpfile
echo "set logscale x"				>> $gpfile 
echo "set logscale y"				>> $gpfile 
echo "set output \"$inputfile.eps\""		>> $gpfile

echo -n "plot	" 				>> $gpfile

first=1

for arch in $archlist
do
	for h in $hashlist
	do
		if [ $first = 0 ] 
		then
			echo -n "  , " >> $gpfile
		else
			first=0
		fi

		echo -n " \"$inputfile.$arch.$h\" using 3:4  title \"arch $arch hash $h\" " >> $gpfile
	done
done

gnuplot $gpfile
