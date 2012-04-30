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


trace_model()
{
	inputfile=$1
	
	cpuentries=`head -1 $inputfile`
	gpuentries=`head -2 $inputfile|tail -1`
	
	cpumodel=`head -3 $inputfile|tail -1`
	gpumodel=`head -4 $inputfile|tail -1`
	
	a_cpu=`cut -f 1 $inputfile| head -5|tail -1`
	b_cpu=`cut -f 2 $inputfile| head -5|tail -1`
	c_cpu=`cut -f 3 $inputfile| head -5|tail -1`
	
	a_gpu=`cut -f 1 $inputfile| head -6|tail -1`
	b_gpu=`cut -f 2 $inputfile| head -6|tail -1`
	c_gpu=`cut -f 3 $inputfile| head -6|tail -1`

	alpha_cpu=`cut -f 5 $inputfile| head -3|tail -1` 
	alpha_gpu=`cut -f 5 $inputfile| head -4|tail -1` 
	
	beta_cpu=`cut -f 6 $inputfile| head -3|tail -1` 
	beta_gpu=`cut -f 6 $inputfile| head -4|tail -1` 
	
	tail -$(($gpuentries + $cpuentries)) $inputfile | head -$(($cpuentries)) |cut -f 2-4 > $inputfile.cpu
	tail -$(($gpuentries)) $inputfile | cut -f 2-4> $inputfile.gpu
	
	echo "pouet $cpuentries gpu $gpuentries toot"
	
	echo "cpumodel $alpha_cpu * size ^ $beta_cpu"
	echo "gpumodel $alpha_gpu * size ^ $beta_gpu"
	
	gpfile=$inputfile.gp
	
	echo "#!/usr/bin/gnuplot -persist" 		> $gpfile
	echo "set term postscript eps enhanced color" 	>> $gpfile
	echo "set logscale x"				>> $gpfile 
	echo "set logscale y"				>> $gpfile 
	echo "set key left top"				>> $gpfile 
	echo "set title \"$inputfile\""			>> $gpfile 
	echo "set output \"$inputfile.eps\""		>> $gpfile
	
	echo  "plot	$alpha_gpu*x**$beta_gpu title \"GPU regression\" ,\\" >> $gpfile
	echo  "	\"$inputfile.gpu\" with errorbar title \"GPU measured\" ,\\" >> $gpfile
	echo  "	$c_gpu + exp(log($a_gpu) + $b_gpu * log(x) ) title \"GPU regression (non linear)\" ,\\" >> $gpfile
	echo  "	\"$inputfile.cpu\" with errorbar title \"CPU measured\" ,\\" >> $gpfile
	echo  "	$alpha_cpu*x**$beta_cpu title \"CPU regression\" ,\\" >> $gpfile
	echo  "	$c_cpu + exp(log($a_cpu) + $b_cpu * log(x) ) title \"CPU regression (non linear)\"" >> $gpfile
	
	gnuplot $gpfile
}

for file in $@
do
	trace_model "$file"
done
