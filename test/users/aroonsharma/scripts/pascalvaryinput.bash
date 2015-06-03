#!/bin/bash
nl=8
blocksize=16

echo "pascal vary input"
echo nl=$nl
echo blocksize=$blocksize

#compile pascal
chpl --fast pascal.chpl -o pascal

for n1 in {600..10000..100}
	do
		n2=$((n1+3))
		
		echo "Cyclic (C) n1=$n1, n2=$n2"
		./pascal -nl $nl --dist=C --n1=$n1 --n2=$n2 --messages --timeit

		echo "Cyclic with modulo unrolling (CM) n1=$n1, n2=$n2"
		./pascal -nl $nl --dist=CM --n1=$n1 --n2=$n2 --messages --timeit

		echo "Block Cyclic (BC) n1=$n1, n2=$n2"
		./pascal -nl $nl --dist=BC --n1=$n1 --n2=$n2 --messages --timeit --blocksize=$blocksize

		echo "Block Cyclic with modulo unrolling (BCM) n1=$n1, n2=$n2"
		./pascal -nl $nl --dist=BCM --n1=$n1 --n2=$n2 --messages --timeit --blocksize=$blocksize
	done