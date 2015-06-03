#!/bin/bash
nl=8
M=10000
TSTEPS=100

echo "jacobi-1d vary block size"
echo nl=$nl
echo M=$M
echo TSTEPS=$TSTEPS

#compile jacobi-1d
chpl --fast jacobi-1d.chpl -o jacobi-1d

for b in {2..20..1}
	do
		
		echo "Block Cyclic (BC) bsize=$b"
		./jacobi-1d -nl $nl --dist=BC --M=$M --TSTEPS=$TSTEPS --bsize=$b --messages --timeit

		echo "Block Cyclic with modulo unrolling (BCM) bsize=$b"
		./jacobi-1d -nl $nl --dist=BCM --M=$M --TSTEPS=$TSTEPS --bsize=$b --messages --timeit
	done