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


# generate the input data
./starpu_fxt_tool $1

# generate the gnuplot script 
echo "#!/usr/bin/gnuplot -persist" 			> histo.gp
echo "set term postscript eps enhanced color" 		>> histo.gp
echo "set output \"histo.eps\""				>> histo.gp
echo "set boxwidth 0.75 absolute" 			>> histo.gp
echo "set boxwidth 0.75 absolute"			>> histo.gp
echo "set nokey"			>> histo.gp
echo "set style fill   solid 1.00 border -1"		>> histo.gp
#echo "set key invert samplen 4 spacing 1 width 0 height 0"	>> histo.gp
#echo "set style histogram rowstacked title  offset character 0, 0, 0"	>> histo.gp
echo "set style histogram rowstacked"	>> histo.gp
echo "set style data histograms"			>> histo.gp
echo "set xtics border in scale 1,0.5 nomirror rotate by -45  offset character 0, 0, 0"	>> histo.gp

#and the actual plotting part :
ncols=`head -1 data |wc -w`

color="red"

echo "plot \\"		>> histo.gp
for i in `seq 1 $(($ncols - 2))`
do

if [ $color == "red" ]; 
then
	color="green"
else
	color="red"
fi


echo "\"data\" using $i lt rgb \"$color\",\\"	>> histo.gp
done

if [ $color == "red" ]; 
then
	color="green"
else
	color="red"
fi


echo "\"data\" using $(($ncols - 1)) lt rgb \"$color\""	>> histo.gp

# now call the script
gnuplot histo.gp
