#!/bin/bash
nl=8
epsilon=0.05
n=200

echo nl=$nl
echo n=$n
echo epsilon=$epsilon

#compile stencil9
chpl --fast stencil9.chpl -o stencil9

echo 'Block (B)'
./stencil9 -nl $nl --dist=B --n=$n --epsilon=$epsilon --messages --timeit

echo 'Cyclic (C)'
./stencil9 -nl $nl --dist=C --n=$n --epsilon=$epsilon --messages --timeit

echo 'Cyclic with modulo unrolling (CM) aggregation minimum = 4'
./stencil9 -nl $nl --dist=CM --n=$n --epsilon=$epsilon --correct --messages --timeit --minimumForAggregation=4

echo 'Cyclic with modulo unrolling (CM) aggregation minimum = 16'
./stencil9 -nl $nl --dist=CM --n=$n --epsilon=$epsilon --correct --messages --timeit --minimumForAggregation=16

echo 'Cyclic with modulo unrolling (CM) aggregation minimum = 100'
./stencil9 -nl $nl --dist=CM --n=$n --epsilon=$epsilon --correct --messages --timeit --minimumForAggregation=100

echo 'Cyclic with modulo unrolling (CM) aggregation minimum = 1000000'
./stencil9 -nl $nl --dist=CM --n=$n --epsilon=$epsilon --correct --messages --timeit --minimumForAggregation=1000000

echo 'No distribution (NONE)'
./stencil9 -nl $nl --dist=NONE --n=$n --timeit
