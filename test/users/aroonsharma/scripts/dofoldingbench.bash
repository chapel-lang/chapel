#!/bin/bash
nl=8
iterations=10
n=50400

echo nl=$nl
echo n=$n
echo iterations=$iterations

#compile folding
chpl --fast folding.chpl -o folding

echo 'Block (B)'
./folding -nl $nl --dist=B --n=$n --iterations=$iterations --messages --timeit

echo 'Cyclic (C)'
./folding -nl $nl --dist=C --n=$n --iterations=$iterations --messages --timeit

echo 'Cyclic with modulo unrolling (CM) aggregation minimum = 4'
./folding -nl $nl --dist=CM --n=$n --iterations=$iterations --correct --messages --timeit --minimumForAggregation=4

echo 'Cyclic with modulo unrolling (CM) aggregation minimum = 16'
./folding -nl $nl --dist=CM --n=$n --iterations=$iterations --correct --messages --timeit --minimumForAggregation=16

echo 'Cyclic with modulo unrolling (CM) aggregation minimum = 100'
./folding -nl $nl --dist=CM --n=$n --iterations=$iterations --correct --messages --timeit --minimumForAggregation=100

echo 'Cyclic with modulo unrolling (CM) aggregation minimum = 1000000'
./folding -nl $nl --dist=CM --n=$n --iterations=$iterations --correct --messages --timeit --minimumForAggregation=1000000

#won't work for this benchmark because of strided access patterns
#echo 'Block Cyclic (BC)'
#./folding -nl $nl --dist=BC --n=$n --iterations=$iterations --bsize=$bsize --messages
#./folding -nl $nl --dist=BC --n=$n --iterations=$iterations --bsize=$bsize --timeit

#echo 'Block Cyclic with modulo unrolling (CM)'
#./folding -nl $nl --dist=BCM --n=$n --iterations=$iterations --bsize=$bsize --correct
#./folding -nl $nl --dist=BCM --n=$n --iterations=$iterations --bsize=$bsize --messages
#./folding -nl $nl --dist=BCM --n=$n --iterations=$iterations --bsize=$bsize --timeit

echo 'No distribution (NONE)'
./folding -nl $nl --dist=NONE --n=$n --iterations=$iterations --timeit
