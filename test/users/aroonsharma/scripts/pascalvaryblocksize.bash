#!/bin/bash
nl=8
n1=100000
n2=100003

echo "pascal vary block size"
echo nl=$nl
echo n1=$n1
echo n2=$n2

#compile pascal
chpl --fast pascal.chpl -o pascal

for b in {2..20..1}
	do
		
		echo "Block Cyclic (BC) blocksize=$b"
		./pascal -nl $nl --dist=BC --n1=$n1 --n2=$n2 --messages --timeit --blocksize=$b

		echo "Block Cyclic with modulo unrolling (BCM) blocksize=$b"
		./pascal -nl $nl --dist=BCM --n1=$n1 --n2=$n2 --messages --timeit --blocksize=$b
	done