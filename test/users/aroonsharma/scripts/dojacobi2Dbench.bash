#!/bin/bash
nl=8
epsilon=0.05
n=400

echo nl=$nl
echo n=$n
echo epsilon=$epsilon

#compile jacobi-2d
chpl --fast jacobi-2d.chpl -o jacobi-2d

echo 'Block (B)'
./jacobi-2d -nl $nl --dist=B --n=$n --epsilon=$epsilon --messages
./jacobi-2d -nl $nl --dist=B --n=$n --epsilon=$epsilon --timeit

echo 'Cyclic (C)'
./jacobi-2d -nl $nl --dist=C --n=$n --epsilon=$epsilon --messages
./jacobi-2d -nl $nl --dist=C --n=$n --epsilon=$epsilon --timeit

echo 'Cyclic with modulo unrolling (CM) aggregation minimum = 4'
./jacobi-2d -nl $nl --dist=CM --n=$n --epsilon=$epsilon --correct --minimumForAggregation=4
./jacobi-2d -nl $nl --dist=CM --n=$n --epsilon=$epsilon --messages --minimumForAggregation=4
./jacobi-2d -nl $nl --dist=CM --n=$n --epsilon=$epsilon --timeit --minimumForAggregation=4

echo 'Cyclic with modulo unrolling (CM) aggregation minimum = 16'
./jacobi-2d -nl $nl --dist=CM --n=$n --epsilon=$epsilon --correct --minimumForAggregation=16
./jacobi-2d -nl $nl --dist=CM --n=$n --epsilon=$epsilon --messages --minimumForAggregation=16
./jacobi-2d -nl $nl --dist=CM --n=$n --epsilon=$epsilon --timeit --minimumForAggregation=16

echo 'Cyclic with modulo unrolling (CM) aggregation minimum = 100'
./jacobi-2d -nl $nl --dist=CM --n=$n --epsilon=$epsilon --correct --minimumForAggregation=100
./jacobi-2d -nl $nl --dist=CM --n=$n --epsilon=$epsilon --messages --minimumForAggregation=100
./jacobi-2d -nl $nl --dist=CM --n=$n --epsilon=$epsilon --timeit --minimumForAggregation=100

echo 'Cyclic with modulo unrolling (CM) aggregation minimum = 1000000'
./jacobi-2d -nl $nl --dist=CM --n=$n --epsilon=$epsilon --correct --minimumForAggregation=1000000
./jacobi-2d -nl $nl --dist=CM --n=$n --epsilon=$epsilon --messages --minimumForAggregation=1000000
./jacobi-2d -nl $nl --dist=CM --n=$n --epsilon=$epsilon --timeit --minimumForAggregation=1000000

echo 'No distribution (NONE)'
./jacobi-2d -nl $nl --dist=NONE --n=$n --timeit
