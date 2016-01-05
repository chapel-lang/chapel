#!/bin/bash
nl=8
M=10000
bsize=4
TSTEPS=100

echo nl=$nl
echo M=$M
echo bsize=$bsize
echo TSTEPS=$TSTEPS

#compile jacobi-1d
chpl --fast jacobi-1d.chpl -o jacobi-1d

echo 'Cyclic (C)'
./jacobi-1d -nl $nl --dist=C --M=$M --TSTEPS=$TSTEPS --messages --timeit

echo 'Cyclic with modulo unrolling (CM) aggregation minimum = 4'
./jacobi-1d -nl $nl --dist=CM --M=$M --TSTEPS=$TSTEPS --correct --messages --timeit --minimumForAggregation=4

echo 'Cyclic with modulo unrolling (CM) aggregation minimum = 16'
./jacobi-1d -nl $nl --dist=CM --M=$M --TSTEPS=$TSTEPS --correct --messages --timeit --minimumForAggregation=16

echo 'Cyclic with modulo unrolling (CM) aggregation minimum = 100'
./jacobi-1d -nl $nl --dist=CM --M=$M --TSTEPS=$TSTEPS --correct --messages --timeit --minimumForAggregation=100

echo 'Cyclic with modulo unrolling (CM) aggregation minimum = 1000000'
./jacobi-1d -nl $nl --dist=CM --M=$M --TSTEPS=$TSTEPS --correct --messages --timeit --minimumForAggregation=1000000

echo 'Block Cyclic (BC)'
./jacobi-1d -nl $nl --dist=BC --M=$M --TSTEPS=$TSTEPS --bsize=$bsize --messages --timeit

echo 'Block Cyclic with modulo unrolling (CM)'
./jacobi-1d -nl $nl --dist=BCM --M=$M --TSTEPS=$TSTEPS --bsize=$bsize --correct --messages --timeit

echo 'Block (B)'
./jacobi-1d -nl $nl --dist=B --M=$M --messages --timeit

echo 'No distribution (NONE)'
./jacobi-1d -nl $nl --dist=NONE --M=$M --timeit
